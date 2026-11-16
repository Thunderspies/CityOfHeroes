#############################################################
## ProtectHolds.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Sound Firehand_loop 70.0 70.0 .45
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		Sound flasharrowhit 70 70 .85
	End

#	Event
#		Type	Local
#		At	root
#		geom	HeatWaveSphere2
#		bhvr	V_COV\Powers\ThermalRadiation\ScaleUp.bhvr
#		LifeSpan 25
#	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	InitialSwordFire
		Type	Local
		At	Head
		bhvr	V_COV\Powers\ThermalRadiation\Fadeinandout.bhvr
		Part	:FireHands.part
		Part	:FireHands_Alpha.part
		Part	:FireHandzSparks.part
	End
End

#############################################################

End