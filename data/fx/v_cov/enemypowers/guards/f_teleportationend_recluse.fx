#########################################################
##
##

FxInfo
LifeSpan  500

##################################

Condition
	On Time
	Time 0


	Event
		EName	TeleStandard
		Type	Local
		At	Root

		ChildFX	:F_Teleport_RecluseStandard.fx
	End

	Event
		Type	positonly
		At	uarmL
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRed.fx
		lookat	LarmL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LarmL
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRed.fx
		lookat	handL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	uarmR
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRed.fx
		lookat	LarmR
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LarmR
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRed.fx
		lookat	handR
		bhvroverride
			stretch 1
		end
	End

############## Legs

	Event
		Type	positonly
		At	ulegL
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	LlegL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LlegL
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	footL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	ulegR
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	LlegR
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LlegR
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	footR
		bhvroverride
			stretch 1
		end
	End

###### torso

	Event
		Type	positonly
		At	head
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	hips
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	ulegR
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	uarmL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	ulegL
		childfx	V_COV\EnemyPowers\Guards\F_TeleportationEnd_RecluseRedLegs.fx
		lookat	uarmR
		bhvroverride
			stretch 1
		end
	End


End

#########################################################

end
