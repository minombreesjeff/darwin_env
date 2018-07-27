/* getmmdb.c */

/* Get MMDB-derived Kinemages, PDB Files, Summaries using PDB acession numbers */

#include <ncbi.h>  
#include <asn.h>    
#include <mmdbapi.h>   /* the MMDBAPI header */

 

#define NUMARGS 6

Args myargs[NUMARGS] = {
        {"Input PDB Accession ",NULL,NULL,NULL,FALSE,'i',ARG_STRING,0.0,0,NULL},
	{"OUTPUT Type \n0 = Summary File \n1 = PDB File, \n2 = Kinemage File", 
	"1","0","2",FALSE,'t',ARG_INT,0.0,0,NULL},
	{"Output File Name",NULL,NULL,NULL,TRUE,'o',ARG_FILE_OUT,0.0,0,NULL},
	{"Model Level \n0 = All Atom; 1=BackBone; 2=PDB models; 3=Vector; ","0","0","3",TRUE,'m',ARG_INT,0.0,0,NULL},
        {"Kinemage Color Type \n0=Default, 1=By Number, 2=By Type, 3=By Temp, 4=By Atom", 
	"0","0","4",FALSE,'c',ARG_INT,0.0,0,NULL},
	{"Kinemage Rendering \n0=V+HET; 11=...+BB; 15=...+RES; 31=...+ALT; 63=...+NMR ", 
	"0","0","63",FALSE,'r',ARG_INT,0.0,0,NULL}

      };


 

Nlm_Int2 Main ()
{
  Int2 iTest;
  Byte bRender;
  Int2 iColor; 
  PDNMS pdnmsModelstruc;
  BiostrucPtr pbsBiostruc;
  Char cOut[254];
  FILE *pFile;
  
        ErrSetFatalLevel(SEV_MAX);
        
   	if (! GetArgs("GetMMDB",NUMARGS,myargs))
		return 1;
	 
	/* Initialize MMDBAPI  */ 

 	if (! OpenMMDBAPI(0, NULL)) 
	  {
	        printf("Have not opened mmdbapi");
	        return 2;	
	  }
	  
	/* load an ASN.1 Biostruc */
	/* Any other method of obtaining a Biostruc is fine... */
	/* This one does a built-in lookup of PDB accesion numbers */
	/* then fetches the file using the Entrez clinet-server interface */
	pbsBiostruc = FetchBiostrucPDB(myargs[0].strvalue, myargs[3].intvalue, 100);
        if (pbsBiostruc == NULL)
	  {
	        printf("Have not fetched Biostruc");
		return 3;
	  }

	/* convert it into a Modelstruc pointed to by pdnmsModelstruc */
	pdnmsModelstruc= MakeAModelstruc(pbsBiostruc);		
	if ( pdnmsModelstruc == NULL )
	  {
	        printf("Have not converted Biostruc");
	        return 4;
	  }
		 
        if (myargs[1].intvalue == 0) /* Summary */
	  {
	     if (!myargs[2].strvalue)
	      { 
		sprintf(cOut,"%s.sum",  myargs[0].strvalue);	
		pFile = fopen(cOut, "w");
	      }   
	    else  
	       pFile = fopen(myargs[2].strvalue,"w");
	    iTest = WriteStructSummary(pdnmsModelstruc,pFile); 
	  }
	 
	if (myargs[1].intvalue == 2) /* Kinemage */
	  {
	    bRender = (Byte) myargs[5].intvalue;
	    iColor = (Int2) myargs[4].intvalue;
	    if (!myargs[2].strvalue)
	      { 
		sprintf(cOut,"%s.kin",  myargs[0].strvalue);	
		pFile = fopen(cOut, "w");
	      }   
	    else  
	       pFile = fopen(myargs[2].strvalue,"w");
	    iTest = WriteKinAllModel(pdnmsModelstruc,pFile,
					iColor,bRender);
					
	  }
		    
        if (myargs[1].intvalue == 1) /* PDB */
	  {
	    if (!myargs[2].strvalue)
	      { 
		sprintf(cOut,"%s.pdb",  myargs[0].strvalue);	
		pFile = fopen(cOut, "w");
	      }   
	    else  
	       pFile = fopen(myargs[2].strvalue,"w");
	    iTest = WritePDBAllModel(pdnmsModelstruc,pFile);
	  }
	
	/* Free the Modelstruc (and its enclosed Biostruc) */	  
 	/* FreeAModelstruc(PDNMS pdnmsThis); not necessary */
	/* This can be done individually - but all Modelstrucs */
        /* remaining are freed in CloseMMDBAPI() */

	/* Shut Down MMDBAPI */	

	CloseMMDBAPI();	

 	return TRUE;
}
