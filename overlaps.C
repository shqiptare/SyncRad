
#include <TGeoManager.h>
#include <TGeoOverlap.h>
#include <TGDMLParse.h>

void overlaps(TString fname="LHC_IR_5_LR",TString ext="root")
{
    gSystem->Load("libGeom");
    gSystem->Load("libGdml");

    if (ext=="root") {
        fname=fname+"."+ext;
        TGeoManager::Import(fname);
        gGeoManager->CheckOverlaps(0.00001);
        gGeoManager->PrintOverlaps();
        gGeoManager->GetTopVolume()->Draw("ogl");
    }

    if (ext=="gdml") {
        TGeoManager* geo = new TGeoManager();
        TGDMLParse   parse;
        fname=fname+"."+ext;
        TGeoVolume* top_vol = parse.GDMLReadFile(fname);
        geo->SetTopVolume(top_vol);
        geo->CheckOverlaps(0.00001);
        geo->PrintOverlaps();
        top_vol->Draw("ogl");
    }

}




