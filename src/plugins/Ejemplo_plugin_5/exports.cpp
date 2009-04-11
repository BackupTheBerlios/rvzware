/*
 * CAPAWARE 3D Geographical Multilayer Framework Free Software
 * Copyright (C) 2009 Instituto Tecnol�gico de Canarias (ITC) and Universidad de Las Palmas de Gran Canaria (ULPGC)
 *
 * This file is part of CAPAWARE.
 *
 * CAPAWARE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * CAPAWARE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The CAPAWARE development team
*/
#include "../../../apps/capaware/PluginInterface.h"

#include <wx/wx.h>
#include <sstream>
#include <cpw/debugger.h>

#include <cpw/graphic/INavigator.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/persistent/file/PersistentFileCallBack.h>
#include <cpw/entity/Element3D.h>
#include <cpw/callback/Model3DCallBack.h>
#include <cpw/persistent/file/PersistentFileCallBack.h>
#include <cpw/common/types.h>

#include "FireSimulation.h"
#include "FireSimulationController.h"
#include "UITest.h"


#define OFFSET_Z	2

class simulacion {
public:
	simulacion(long t, double _x, double _y): time(t), x(_x), y(_y) {}
	long time;
	double x, y;
};

std::vector <simulacion> simul;

using namespace cpw;

// para poder hacer el printf
#include <cpw/common/ApplicationLog.h>
#define printf cpw::ApplicationLog::GetInstance()->GetLogger()->printf


// calcula la diferencia de tiempo entre los instantes t1 y t2
long int diff_time (cpwTime t1, cpwTime t2) {
	long int m1 = 1000*t1.seconds + t1.miliseconds;
	long int m2 = 1000*t2.seconds + t2.miliseconds;
	return (m2-m1);
}

// calcula el vector gradiente del terreno en la posici�n actual
void ComputeGradient(cpw::IScene *scene, double pos[3], double grad[2])
{
	const double h = 0.5;
	cpw::Point3d<double> px, py;

	// estimamos las derivadas parciales en X y en Y
	scene->IntersectRayWithTerrain (cpw::Point3d<double>(pos[0]+h, pos[1], 10000.0f),
			                             cpw::Point3d<double>(pos[0]+h, pos[1], -10000.0f),
										 px, true);
	scene->IntersectRayWithTerrain (cpw::Point3d<double>(pos[0], pos[1]+h, 10000.0f),
			                             cpw::Point3d<double>(pos[0], pos[1]+h, -10000.0f),
										 py, true);
	grad[0] = (px.z-(pos[2]-OFFSET_Z)) / h;
	grad[1] = (py.z-(pos[2]-OFFSET_Z)) / h;

}

// calcula el vector gradiente del terreno en la posici�n actual
void ComputeGradientFromXY(cpw::IScene *scene, double pos[3], double grad[2])
{
	const double h = 0.5;
	cpw::Point3d<double> px, py, p0;

	// estimamos las derivadas parciales en X y en Y
	scene->IntersectRayWithTerrain (cpw::Point3d<double>(pos[0], pos[1], 10000.0f),
			                             cpw::Point3d<double>(pos[0], pos[1], -10000.0f),
										 p0, true);
	scene->IntersectRayWithTerrain (cpw::Point3d<double>(pos[0]+h, pos[1], 10000.0f),
			                             cpw::Point3d<double>(pos[0]+h, pos[1], -10000.0f),
										 px, true);
	scene->IntersectRayWithTerrain (cpw::Point3d<double>(pos[0], pos[1]+h, 10000.0f),
			                             cpw::Point3d<double>(pos[0], pos[1]+h, -10000.0f),
										 py, true);
	grad[0] = (px.z-p0.z) / h;
	grad[1] = (py.z-p0.z) / h;

	// devolvemos en pos[2] el valor z
	pos[2] = p0.z;
}




class GraphicE3DCallBack : public Model3DCallBack
{
	public:
		GraphicE3DCallBack(cpw::IScene *appscene): scene(appscene){}
		GraphicE3DCallBack(const GraphicE3DCallBack &gfcb) {}
		~GraphicE3DCallBack(void) {}
		
		virtual void Insert(const Callable* callable) {}
		virtual void Update(Callable* callable) {}
		virtual void Delete(Callable* callable) {}


		virtual void Animate(Callable* callable, cpwTime &time_stamp){
			
			// si no se ha movido el tiempo no hacemos nada
			static cpwTime last_time = time_stamp;
			if (diff_time (last_time, time_stamp) == 0) return;
			last_time = time_stamp;

			// tomamos el elemento
			cpw::Element3D *e3d = (cpw::Element3D *) callable;
			
			// calculamos la duraci�n entre el instante actual y el inicial
			long int mseg = diff_time (e3d->GetStartTime(), time_stamp);
			int seg = mseg / 1000;

			// actualizamos la posici�n
			double pos[3];
			if (seg+1<simul.size()) {
				pos[0] = (double) (mseg-1000*seg)/1000 * (simul[seg+1].x-simul[seg].x) + simul[seg].x;
				pos[1] = (double) (mseg-1000*seg)/1000 * (simul[seg+1].y-simul[seg].y) + simul[seg].y;
			} else {
				pos[0] = simul[simul.size()-1].x;
				pos[1] = simul[simul.size()-1].y;
			}
			// actualizamos la coordenada z para que no se hunda en el terreno
			cpw::Point3d<double> i_point;
			if (scene->IntersectRayWithTerrain(cpw::Point3d<double>(pos[0], pos[1], 10000.0f),
			                             cpw::Point3d<double>(pos[0], pos[1], -10000.0f),
										 i_point, true))
		    {

					pos[2] = i_point.z + OFFSET_Z;
		    }

			// fijamos la nueva posici�n
			e3d->SetUtmd (pos);
			e3d->GraphicUpdate();
		}
		
		virtual void Visualize(Callable* callable, const bool &value) {}

		//GraphicFireCallBack &operator = (const GraphicFireCallBack &gfcb){};
		virtual ICallBack* Clone() {return new GraphicE3DCallBack(*this);}

	private:		
		cpw::IScene *scene;
};



      


//--------------------------------------------------------
class CMyApp : public wxApp
{
public:
    bool OnInit();

private:
};

IMPLEMENT_APP_NO_MAIN(CMyApp)

BOOL APIENTRY DllMain(HMODULE Module,DWORD Reason,LPVOID)
{
        if(Reason==DLL_PROCESS_DETACH)
        {			
			if( wxTheApp )
				wxTheApp->OnExit();
			wxEntryCleanup();
        }
        else if(Reason==DLL_PROCESS_ATTACH)
        {
                int argc = 0;
                char** argv = NULL;
                if(!wxEntryStart(argc, argv) ||
                        !wxTheApp ||
                        !wxTheApp->CallOnInit())
                {
					wxEntryCleanup();
					if( wxTheApp )
						wxTheApp->OnExit();

                    return FALSE;
                }
        }
       
        return TRUE;
}

bool CMyApp::OnInit()
{       
    return true;
};

void EntitiesToRegister(std::vector<cpw::Entity*> &v)
{
}

//--------------------------------------------------------


void RegisterMenu(std::string &menu)
{
	menu = "Ejemplo 5: Interfaz WxWidget";
}

void ExecPlugin(std::vector<cpw::Entity*> &ventity, cpw::ApplicationScene *appscene, 
				const std::string &entity_path, const std::string &icon_path, 
				const std::string &data_path, const std::string &language, wxWindow *parent, 
				cpw::INavigatorManager *navigator_manager, cpw::IStatusController *istatus_controller)
{	
	float x, y, z;
	int cx, cy;

	// obtenemos la intersecci�n del centro de pantalla con el terreno
	cpw::INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
	navigator->GetCenterScreenCoords (cx, cy);
	navigator->IntersectMouseWithScene (cx, cy, x, y, z);

	// creamos una nueva excavadora en ese punto
	cpw::Element3D *el = new cpw::Element3D;
	el->AddCallBack(new cpw::Model3DCallBack);
	el->SetPersistentCallBack(new cpw::PersistentFileCallBack);
	el->SetName("Elemento3D_plugin");
	el->SetUtm(x, y, z);
	el->SetOrientation(0, 0, -1);
	el->SetScale(1,1,1);
	el->SetIcon("../../data/icons/agua.png");
	el->SetModelUrl("../../data/models/misc/bola_azul.osg");

	// insertamos a mano el elemento en la escena para poder verlo 
	el->GraphicInsertAndRegister();

	// lanzamos la ventana de la interfaz de usuario
	UITest *ui = new UITest (ventity, navigator_manager, el->GetUtmd(0), el->GetUtmd(1), 
					parent, wxID_ANY, wxString(_T("Element")), 
					wxDefaultPosition, wxSize(390, 360)); 
	ui->ShowModal();

	// quitamos el elemento que insertamos a mano
	el->GraphicDeleteAndUnregister();
	
	// miramos si se puls� el bot�n OK
	if (ui->GetReturnCode() == wxID_OK) {

		// obtenemos la posici�n actual del elemento
		double pos[3] = {el->GetUtmd(0), el->GetUtmd(1), el->GetUtmd(2)};
		printf ("    ");
		printf ("COMIENZO SIMULACION: %d segundos", ui->GetSeconds());
		simulacion s(0, pos[0], pos[1]);
		simul.push_back(s);

		// iteramos tantos segundos seg�n indicado por el usuario
		for (int n=1; n<ui->GetSeconds(); n++) {

			// calculamos el gradiente en la posici�n actual
			double grad[2];
			ComputeGradientFromXY (appscene->GetScene(), pos, grad);
			printf ("Segundo %d: Objeto en (%.3f, %.3f, %.3f)", n-1, pos[0], pos[1], pos[2]);

			// actualizamos su posici�n
			pos[0] -= 20*grad[0];
			pos[1] -= 20*grad[1];

			// lo a�adimos al vector
			s.time = n;
			s.x = pos[0];
			s.y = pos[1];
			simul.push_back(s);
		}

		// vamos a animarla
		el->SetAnimateCallBack(new GraphicE3DCallBack(appscene->GetScene()));

		// par�metros para el panel de animaci�n
		cpwTime tim;
		tim.seconds = 0;
		tim.miliseconds = 0;
		el->SetStartTime(tim);
		tim.seconds = ui->GetSeconds();
		el->SetEndTime(tim);

		// insertamos el elemento en la lista de entidades de salida
		ventity.push_back(el);
	}
	else
		printf ("se cancelo la simulacion");


}
