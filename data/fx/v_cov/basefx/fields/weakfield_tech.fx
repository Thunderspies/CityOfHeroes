#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound kinetic4 80 80 .84
	End
End

#####################  Glows  ######################

Condition
	On 	Time
	Time 	0

	Event
		EName 	grime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	1.9 3.7 -.1
		End
		Part1	:EnergyGlow02.part
	End	

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	grime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-1.9 3.7 -.1
		End
		Part1	:EnergyGlow02.part
	End	

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	grime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 4.5 -.1
		End
		Part1	:EnergyGlow02.part
	End	

End



#########################################################
#################### AOE Indicator ########################
#########################################################

Condition
	

	Event
		EName	Center
		Type	Local
		At 	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 4.25 0
		End
	End

End

Condition
	On	Time
	Time	25
	
	Event
		Type	local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 2.5 0
		End
		Pmagnet	Center

		Part	:EnergyGlow01.part
		Part	:WeaknessGlow01.part
	End

End

Condition
	On	Cycle
	Time	27

	Event
		Type	Local
		At	Center

		Geom	EarthGraspHit

		bhvr	:EnergyDrainScale.bhvr
		LifeSpan	27
		
	End

End

	

Condition
	On	Cycle
	Time	31

	Event
		Type	Local
		At	Center

		Geom	EarthGraspHit

		bhvr	:EnergyDrainScale.bhvr
		LifeSpan	32
		
	End

End

Condition
	On 	Cycle
	Time 	17

	Event	

		ENAME	Base
		Type	local
		At	root
		
		Bhvr 	:DarkScale3.bhvr
		Geom	FX_DarkWave
		LifeSpan	20

	End

End

End