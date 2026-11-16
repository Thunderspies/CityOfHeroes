#############################################################
## PsionicShockwave_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FS_PsionicShockwave_Hit_01 100 100 .8
	End

	Event
		Type	Local
		At	Head
		Part	:PsionicShockwave_Globs_Hit.part
		Part	:PsionicShockwave_Globs_Alpha_Hit.part
		Lifespan	5
	End

	Event
		Type	Local
		At	Head
		Part	:Head_Glow_Growing.part
		Part	:Head_Shockwave.part
		Part	:Head_Rings_Growing.part
		Part	:Head_Rings_Growing2.part
		Lifespan	10
	End

End

#############################################################

End