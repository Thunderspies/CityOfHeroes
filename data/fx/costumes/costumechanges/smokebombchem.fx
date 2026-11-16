#########################################################
##
##

FxInfo


Condition
	On 	Time
	Time 	19

	Event
		EName 	Flash3
		Type	Local
		At	Root
		Part	Costumes\CostumeChanges\SmokePillarChemAdditive.part
		part	Costumes\CostumeChanges\SmokePillarChem.part
		part	Costumes\CostumeChanges\SmokePillarChem2.part
		Part	Costumes\CostumeChanges\LargeBlast2.part
		Part	Costumes\CostumeChanges\LowerBlastChem.part
		Part	Costumes\CostumeChanges\Sparks.part
		Part	Costumes\CostumeChanges\Embers.part
	End

#################################
######### ROCKET 1 ##############
#################################

	Event
		Ename	Rocket1
		Type	Local
		At 	Root
		Bhvr 	behaviors\FireBurstUP.bhvr
		Lifespan 11

	End


	Event
		Ename 	Spinner1
		Type	Local
		At	Rocket1
		BhvrOverride
			Spin 0.0 0.8 0.0
		End
		Lifespan 11
	End

	Event
		Ename 	Offset
		Type 	Local
		At	Spinner1
		BhvrOverride
			PositionOffset 0.0 0.0 1.5
		End
		Part	Costumes\CostumeChanges\SmokeTrailsAdditive.part
		Part	Costumes\CostumeChanges\SmokeTrails.part
		Part	Costumes\CostumeChanges\SmokeTrails2.part
		Lifespan 11
	End

#################################
######### ROCKET 2 ##############
#################################

	Event
		Ename	Rocket2
		Type	Local
		At 	Root
		Bhvr 	behaviors\FireBurstUP.bhvr
		Lifespan 13

	End

	Event
		Ename 	Spinner2
		Type	Local
		At	Rocket2
		BhvrOverride
			Spin 0.0 1.0 0.0
		End
		Lifespan 13
	End

	Event
		Ename 	Offset2
		Type 	Local
		At	Spinner2
		BhvrOverride
			PositionOffset 0.0 0.0 .5
		End
		Part	Costumes\CostumeChanges\SmokeTrailsAdditive.part
		Part	Costumes\CostumeChanges\SmokeTrails.part
		Part	Costumes\CostumeChanges\SmokeTrails2.part
		Lifespan 13
	End


#################################
######### ROCKET 3 ##############
#################################

	Event
		Ename	Rocket3
		Type	Local
		At 	Root
		Bhvr 	behaviors\FireBurstUP.bhvr
		Lifespan 15

	End

	Event
		Ename 	Spinner3
		Type	Local
		At	Rocket3
		BhvrOverride
			Spin 0.0 .3 0.0
		End
		Lifespan 15
	End

	Event
		Ename 	Offset3
		Type 	Local
		At	Spinner3
		BhvrOverride
			PositionOffset 0.0 0.0 1.5
		End
		Part	Costumes\CostumeChanges\SmokeTrailsAdditive.part
		Part	Costumes\CostumeChanges\SmokeTrails.part
		Part	Costumes\CostumeChanges\SmokeTrails2.part
		Lifespan 15
	End


End