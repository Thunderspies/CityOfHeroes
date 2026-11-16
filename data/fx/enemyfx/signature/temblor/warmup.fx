#############################################################
## TEMBLOR KNOCK UP FX
#############################################################

FxInfo
Lifespan	180
#############################################################

##		TELL

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	Earthquake_Light_01 300 300 .8
		Sound 	Earthquake_Light_02 300 300 .8
		Sound 	Earthquake_Light_03 300 300 .8
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\CameraShake_Subtle90.bhvr
		BhvrOverride
			PositionOffset		0 0.4 0
		End
		Part	:Dust_Ground.part
		Part	:Dust_Ground_Dark.part
		Lifespan	95
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 1 0
		End
		Part	:RockBits_01.part
		Lifespan	90
	End


	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 -6 0
		End
		Part	:Dust_02.part
		Part	:Dust_Dark02.part
		Lifespan	90
	End

	Event
		Ename	ExplosiveForce1
		Type	Start
		At	RadiusFxScale1

		BhvrOverride
			PhysForceType		Up
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	90

	End
End

#############################################################-

End