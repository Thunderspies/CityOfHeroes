#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################


#####################  Glows  ######################

Condition
	On 	Cycle
	Time 	5

	Event
		EName 	grime
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			StartJitter	.6 1 .4
		End
		Part1	:WeaknessGlow02.part
		Lifespan	1
	End	

End

Condition
	On 	Cycle
	Time 	6

	Event
		EName 	grime
		Type	Local
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			StartJitter	.4 0.5 .2
		End
		Part1	:WeaknessGlow02.part
		Lifespan	1
	End	

End

Condition
	On 	Cycle
	Time 	7

	Event
		EName 	grime
		Type	Local
		At	Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			StartJitter	.1 .1 .1
		End
		Part1	:WeaknessGlow02.part
		Lifespan	1
	End	

End

Condition
	On 	Cycle
	Time 	6

	Event
		EName 	grime
		Type	Local
		At	UlegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
			StartJitter	.1 1 .1
		End
		Part1	:WeaknessGlow02.part
		Lifespan	1
	End	

End


Condition
	On 	Cycle
	Time 	6

	Event
		EName 	grime
		Type	Local
		At	UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
			StartJitter	.1 1 .1
		End
		Part1	:WeaknessGlow02.part
		Lifespan	1
	End	

End


Condition
	On 	Cycle
	Time 	5

	Event
		EName 	grime
		Type	Local
		At	LlegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
			StartJitter	.1 1 .1
		End
		Part1	:WeaknessGlow02.part
		Lifespan	1
	End	

End


Condition
	On 	Cycle
	Time 	5

	Event
		EName 	grime
		Type	Local
		At	LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
			StartJitter	.1 1 .1
		End
		Part1	:WeaknessGlow02.part
		Lifespan	1
	End	

End


Condition
	On 	Cycle
	Time 	5

	Event
		EName 	grime
		Type	Local
		At	LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-.5 0 0
			StartJitter	.5 .1 .1
		End
		Part1	:WeaknessGlow02.part
		Lifespan	1
	End	

End


Condition
	On 	Cycle
	Time 	5

	Event
		EName 	grime
		Type	Local
		At	LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	.5 0 0
			StartJitter	.5 .1 .1
		End
		Part1	:WeaknessGlow02.part
		Lifespan	1
	End	

End




#########################################################
#################### AOE Indicator ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event
		Type	local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 3 0
		End
		Pmagnet	Hips

		Part	:WeaknessGlow01small.part
	End

End

End