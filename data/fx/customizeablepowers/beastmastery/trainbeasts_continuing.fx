###########################################################

FxInfo

###########################################################

#Condition
#	On 	Time
#	Time	0
#	Event
#		Type	Local
#		At	Root
#		Sound 	BM_TrainBeasts_Hit_01 100 100 .8
#	End
#End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	CAT
		DoMany	1

		Event
			EName	Cat_TrainBeasts
			Type	Local
			At	Origin
			WHILE	CAT
			Flags	OneAtATime
			Bhvr	behaviors/GenericParticleFade.bhvr
			ChildFX	:CHILD_TrainBeasts_Lion.fx
		End
	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	DOG
		DoMany	1

		Event
			EName	Cog_TrainBeasts
			Type	Local
			At	Origin
			WHILE	DOG
			Flags	OneAtATime
			Bhvr	behaviors/GenericParticleFade.bhvr
			ChildFX	:CHILD_TrainBeasts_Wolf.fx
		End
	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	DIREWOLF
		DoMany	1

		Event
			EName	Cog_TrainBeasts
			Type	Local
			At	Origin
			WHILE	DIREWORLF
			Flags	OneAtATime
			Bhvr	behaviors/GenericParticleFade.bhvr
			ChildFX	:CHILD_TrainBeasts_DireWolf.fx
		End
	End


###########################################################

End
