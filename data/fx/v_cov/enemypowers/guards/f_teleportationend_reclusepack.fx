#########################################################
##	
##

FxInfo
LifeSpan  500



###### Recluse BackPack
#
Condition
	On Time
	Time 0

	
#	Event
#		EName	TeleStandard
#		Type	Local
#		At	Root
#
#		ChildFX	:F_Teleport_Standard.fx
#	End

	Event
		Type	positonly
		At	C_uarmL 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRed.fx
		lookat	C_LarmL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	C_LarmL 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRed.fx
		lookat	C_handL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	C_uarmR 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRed.fx
		lookat	C_LarmR
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	C_LarmR 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRed.fx
		lookat	C_handR
		bhvroverride
			stretch 1
		end
	End

############## Legs
	
	Event
		Type	positonly
		At	C_ulegL 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_LlegL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	C_LlegL 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_footL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	C_ulegR 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_LlegR
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	C_LlegR 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_footR
		bhvroverride
			stretch 1
		end
	End

###### More Legs

	Event
		Type	positonly
		At	C_Fore_ULegL 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_Fore_LLegL 
		bhvroverride
			stretch 1
		end
	End
	
	Event
		Type	positonly
		At	C_Fore_LLegL 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_Fore_FootL 
		bhvroverride
			stretch 1
		end
	End

	Event
		Type	positonly
		At	C_Fore_FootL 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_Fore_ToeL 
		bhvroverride
			stretch 1
		end
	End

####

	Event
		Type	positonly
		At	C_Fore_ULegR 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_Fore_LLegR 
		bhvroverride
			stretch 1
		end
	End
	
	Event
		Type	positonly
		At	C_Fore_LLegR 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_Fore_FootR 
		bhvroverride
			stretch 1
		end
	End

	Event
		Type	positonly
		At	C_Fore_FootR 
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	C_Fore_ToeR 
		bhvroverride
			stretch 1
		end
	End

End




end
