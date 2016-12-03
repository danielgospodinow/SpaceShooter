package org.cocos2dx.cpp;

public enum NetworkFunctions 
{
	  SpawnEnemyShipBlast 
	  {
		  public String toString() 
	      {
	          return "SpawnEnemyShipBlast";
	      }
	  },
	  SendMyShipType
	  {
		  public String toString()
		  {
			  return "SendMyShipType";
		  }
	  },
	  SendMyScreenDetails
	  {
		  public String toString()
		  {
			  return "SendMyScreenDetails";
		  }
	  },
	  SendMyName
	  {
		  public String toString()
		  {
			  return "SendMyName";
		  }
	  },
	  SetOpponentIsReady
	  {
		  public String toString()
		  {
			  return "SetOpponentIsReady";
		  }
	  },
	  RestartOpponentGame
	  {
		  public String toString()
		  {
			  return "RestartOpponentGame";
		  }
	  },
	  LeaveOpponentGame
	  {
		  public String toString()
		  {
			  return "LeaveOpponentGame";
		  }
	  }
}
