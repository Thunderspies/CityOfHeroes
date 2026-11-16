#########################################################
##	Warburg Bomb - Nuke
##

FxInfo
	
lifespan	300





Condition

	On 	Time
	Time 	0
	
	Event
		ENAME	Glow
		Type	SetLight	
		Bhvr	:ChemHitLight.bhvr

		Lifespan 30
	End

	Event
		EName   Splash
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemHitSplash.part
		part	:ChemHitFlash.part
		part	:ChemHitMist.part
		Lifespan	5
	End
	
	Event
		EName   Steam
		Type	Posit
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemHitMist.part
		part	:ChemSmoke.part
		part	:ChemSmoke02.part
		Lifespan	15
	End

End

Condition
	On 	Time
	Time 	3
	Repeat	3
	RepeatJitter	1

	Event
		EName   AcidRainDrop
		Type	Start
		At	Root
		Splat	Generic_Ring.tga
		Bhvr	:ChemRippleSplat.bhvr
		BhvrOverride
			StartJitter 3 0 3
		End
		Lifespan	4
	End

End

End
