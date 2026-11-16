#############################################################
## Inferno.fx
#############################################################

FxInfo

Lifespan 90

Flags IsArmor DontSuppress

#############################################################

Condition
	On 	Time
	Time 	2

	Event
		Type	Start
		At	Root
		ChildFX	:Child_FinalExplosion.fx
		Sound RobotSelfDestructExplode_01 100 100 .95
	End
End

#############################################################

End