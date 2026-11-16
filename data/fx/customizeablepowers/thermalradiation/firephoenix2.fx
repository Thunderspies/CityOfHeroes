#########################################################
##	FireAura
FxInfo


Lifespan	300
###############################################################################

Condition
	On 	Time
	Time	60

	Event
		Type 	Local
		At	Origin
		Sound fireball1 100 100 .85
	End

End

Condition
	On 	Time
	Time	60

	Event
		Type 	Local
		At	Origin
		Sound Firesword_loop 100.0 100.0 .44
	End

End


Condition
	On 	Time
	Time	0

	Event
		Type 	Local
		At	Origin
		Sound torch2_loop 100.0 100.0 .4
	End

End

Condition
	On 	Time
	Time	0

	Event
		Type 	Local
		At	Origin
		Sound rumble1 100.0 100.0 .75
	End

End

Condition
	On 	Time
	Time	0

	Event
		Type 	Local
		At	Origin
		Sound ressurect 100.0 100.0 .7
	End

End



Condition
	On 	Time
	Time	95

	Event
		Type 	Local
		At	Origin
		Sound cinders3 100.0 100.0 .9
	End

End


Condition
	On 	Time
	Time	120

	Event
		Type 	Local
		At	Origin
		Sound aura3 100.0 100.0 .85
	End

End

Condition
	On 	time
	Time	0

	Event
		EName	wing
		Type	Local
		At	T_Root
		Part	:PhoenixDeath.part
		Sound Ignite2 100.0 100.0 .7
		LifeSpan	85
	End

End

Condition
	On 	time
	Time	132

	Event
		EName	wing
		Type	Local
		At	winghook
		AltPiv	1
		Part	:lightRays.part
		Part	:PhoenixDeath1.part


		LifeSpan	78
	End

	Event
		EName	headSparks
		Type	Local
		At	T_head
		Part	:PhoenixSparks.part
		lifeSpan	65

	End

End

Condition
	On 	Time
	Time 	80

	Event
		EName	winghook
		Type	Local
		At	T_Root
		Geom	RoottoHeadAdjustment
		Bhvr	:PhoenixMoveUp.Bhvr
	End

	Event
		EName	Pivot
		Type	Local
		At	T_head
		Part	:FireCore.part

	End

	Event
		EName	Hips2
		Type	Local
		At	T_Hips
		Part	:FireCore2.part
		Part	:FireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	T_Chest
		Part	:FireCore3.part
		Part	:FireCoreTrail2.part
		Part	:FireCoreSparks2.part
	End

	Event
		EName	Chest
		Type	Local
		At	T_Head
		Part	:FireCore4.part
		Part	:FireCoreTrail3.part
	End


	Event
		EName	Lleg
		Type	Local
		At	T_LlegL
		Part	:FireCore1.part
		Part	:FireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	T_LlegR
		Part	CustomizeablePowers\ThermalRadiation\FireCore1.part
		Part	CustomizeablePowers\ThermalRadiation\FireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	T_FootL
		Part	:FireCore1Feet.part
		Part	:FireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	T_FootR
		Part	:FireCore1Feet.part
		Part	:FireCoreTrailFeet.part
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part	:FireHandz.part
		LifeSpan	75

	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part	:FireHandz.part
		LifeSpan	75
	End

End

Condition
	On	Time
	Time	95

	Event
		EName	Hips
		Type	Local
		At	T_Hips
		Part	:FireAura2.part
	End

End

Condition
	On	Time
	Time	120

	Event
		EName	Hips
		Type	Local
		At	T_Hips
		Part	:FireAura2.part
	End
End


