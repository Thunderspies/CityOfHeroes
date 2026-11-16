#########################################################
##	Fire_Ball
##
FxInfo



Condition
	
	Event
		EName	3
		Type	Local
		At	LarmL

		BHVR	Behaviors/GenericParticleFade.bhvr

		ChildFx	:Red_ElectricityChild.fx

		lifespan	20
	End
	
End

Condition
	On 	cycle
	Time 	30
	chance	.8

	Event
		EName	3
		Type	Local
		At	LarmR

		BHVR	Behaviors/GenericParticleFade.bhvr


		ChildFx	:Red_ElectricityChild.fx

		lifespan	20
	End
	
End


End	


			