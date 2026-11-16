#########################################################
##	template

FxInfo

##############################################################################
##Physics
#######################

Condition
	On 	Time
	Time 	0
	Repeat	6
	
	Event

		EName	GasSpurts
		Type	Local
		At	chest
				
		ChildFX	V_COV\PhysicsEnabled\PoisonSplashSmall.fx
		Lifespan	75
	End
	
	Event
		HardwareOnly
		EName	GasSpurts
		Type	Local
		At	chest
				
		ChildFX	V_COV\PhysicsEnabled\PoisonSplashSmall.fx
		Lifespan	75
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	4
	
	Event

		EName	GasHurts
		Type	Local
		At	chest
				
		ChildFX	V_COV\PhysicsEnabled\PoisonQuillScatterSmall.fx
		Lifespan	175
	End
	
	Event
		HardwareOnly
		EName	GasHurts
		Type	Local
		At	chest
				
		ChildFX	V_COV\PhysicsEnabled\PoisonQuillScatterSmall.fx
		Lifespan	175
	End
End

#######################################################################################

#########################################################
##Core
#############################################

Condition
	On 	Time
	Time 	0

	Event

		
		Type	local
		At	neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		Part1	:MustardGasHitA.part
		Part2	:MustardGasSpecksHitA.part
		Part3	:MustardGasBubblesA.part

		LifeSpan	25

	End

	Event

		
		Type	local
		At	chest
		Bhvr	:ThoryBhvr01.bhvr
		Geom	quill01
		LifeSpan	25
	End

	Event

		
		Type	local
		At	chest
		Bhvr	:ThoryBhvr02.bhvr
		Geom	quill01
		LifeSpan	25
		
	End

	Event

		
		Type	local
		At	chest
		Bhvr	:ThoryBhvr03.bhvr
		Geom	quill01
		LifeSpan	25
		
	End

	

	Event

		
		Type	local
		At	chest
		Bhvr	:ThoryBhvr04.bhvr
		Geom	quill01
		LifeSpan	25
	End

	
End

######################################################3

Condition
	On time
	Time 0

	Event

		Type	Local
		At	Chest
		Sound aim2 60 60 .6
		End
end




Condition
	On Time 
	Time 0
	
	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	:YellowUpGlow.part
		Part2	:YellowUpSpecks2.part
		#Sound	clawYellow3 100 30 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End

Condition
	On Time 
	Time 10
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks.part
		Part2	:YellowUpRing01.part
		Part3	:YellowUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 20
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks.part
		Part3	:YellowUpRing02.part
		Part4	:YellowUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 30
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks2.part
		Part2	:YellowUpRingSlow.part
		Part3	:YellowUpRing03.part
		Part4	:YellowUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

End


Condition
	On Time 
	Time 13

	Event
		EName	pow2
		Type 	Destroy
			
	End

End

Condition
	On Time 
	Time 75

	Event
		EName	all
		Type 	Destroy
			
	End

End

	

End	

	