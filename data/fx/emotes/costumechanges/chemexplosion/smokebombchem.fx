#############################################################
## SmokeBombChem.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Flash3
		Type	Local
		At	Root
		Part	:SmokePillarChemAdditive.part
		part	:SmokePillarChem.part
		part	:SmokePillarChem2.part
		Part	:SmokePillarChem2_Additive.part
		Part	:Embers.part
	End

	Event
		EName 	Flash3
		Type	Local
		At	Hips

		Part	:LowerBlastChem.part
		Part	:Sparks.part
		Part	:Sparks_Vertical.part
	End
End

Condition
	On	Time
	Time	1

	Event
		Ename	Rocket1
		Type	Local
		At 	Root
		Bhvr 	:RocketMotion.bhvr
		Lifespan 17
	End

	Event
		Ename 	Spinner1
		Type	Local
		At	Rocket1
		BhvrOverride
			Spin 0.0 .5 0.0
		End
		Lifespan 17
	End

	Event
		Ename 	Offset1
		Type 	Local
		At	Spinner1
		BhvrOverride
			PositionOffset 0.0 0.0 1
		End
		Part	:SmokeTrailsAdditive.part
		Part	:SmokeTrails.part
		Part	:SmokeTrails2.part
		Part	:SmokeTrails3.part
		Lifespan 15
	End

#############################################################

	Event
		Ename	Rocket2
		Type	Local
		At 	Root
		Bhvr 	:RocketMotion.bhvr
		Lifespan 17
	End

	Event
		Ename 	Spinner2
		Type	Local
		At	Rocket2
		BhvrOverride
			Spin 0.0 -.5 0.0
		End
		Lifespan 17
	End

	Event
		Ename 	Offset2
		Type 	Local
		At	Spinner2
		BhvrOverride
			PositionOffset 0.0 0.0 1
		End
		Part	:SmokeTrailsAdditive.part
		Part	:SmokeTrails.part
		Part	:SmokeTrails2.part
		Part	:SmokeTrails3.part

		Lifespan 17
	End

#############################################################

	Event
		Ename	Rocket3
		Type	Local
		At 	Root
		Bhvr 	:RocketMotion.bhvr
		Lifespan 17
	End

	Event
		Ename 	Spinner3
		Type	Local
		At	Rocket3
		BhvrOverride
			Spin 0.0 .5 0.0
		End
		Lifespan 17
	End

	Event
		Ename 	Offset3
		Type 	Local
		At	Spinner3
		BhvrOverride
			PositionOffset 0.0 0.0 1
		End
		Part	:SmokeTrailsAdditive.part
		Part	:SmokeTrails.part
		Part	:SmokeTrails2.part
		Part	:SmokeTrails3.part

		Lifespan 19
	End
End