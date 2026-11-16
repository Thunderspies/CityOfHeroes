#########################################################
##	Warburg Bomb - Nuke
##

FxInfo
	
lifespan	300

Condition

	On 	Cycle
	Time 	1
	
	Event
		EName   Splash
		Type	Start
		BhvrOverride
			StartJitter 45 0 45
		End
		part	:ChemSplash.part
		Lifespan	2
	End

	Event
		EName   AcidRainDrop
		Type	Start
		At	Splash
		Splat	Generic_Ring.tga
		Bhvr	:ChemRippleSplat.bhvr
		part	:ChemSplash.part
		Lifespan	5
	End

End


End
