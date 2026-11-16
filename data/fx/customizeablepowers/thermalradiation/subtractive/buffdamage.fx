#############################################################
## BuffDamage.fx
#############################################################

FxInfo

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Sound Firehand_loop 100.0 100.0 .45
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
#		bhvr	V_COV\Powers\ThermalRadiation\ScaleUp.bhvr
#		geom	HeatWaveSphere2
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
		At	WepR
		bhvr	V_COV\Powers\ThermalRadiation\Fadeinandout.bhvr
		Part	:FireHandz.part
		Part	:FireHandz_Alpha.part

		Part	:FireHandzSparks.part
	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		bhvr	V_COV\Powers\ThermalRadiation\Fadeinandout.bhvr
		Part	:FireHandz.part
		Part	:FireHandz_Alpha.part

		Part	:FireHandzSparks.part
	End
End

#############################################################

End