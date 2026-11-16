#############################################################
## Child_Aim.fx
#############################################################

FxInfo
Lifespan 80
#########################################################

Condition
	On time
	Time 0

	Event
		ENAME   P2
		Type	Local
		At	Origin
#		BhvrOverride
#			Alpha		1
#		End
#		Geom	FlameThrower
		Part	:Rifle_MiniGlows.part
	End

	Event
		Type	Local
		At	P2
		Part	:Rifle_MiniArcs.part
		Lifespan 25
	End
End

#############################################################
End