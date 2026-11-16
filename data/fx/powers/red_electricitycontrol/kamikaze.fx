#########################################################
##	forceBubble
########################################################
FxInfo


LifeSpan	300

########################################################


#############################################################################
## Pre Shock Field
#######################################################

Condition
	On 	Time
	Time 	0

	Event
		
		Type	Local
		At	Hips
		#Part1	:ShockFieldRing.part
		
		LifeSpan	50

	End

	Event
		
		Type	Local
		At	Hips
		Part2	:ShockFieldGlow.part
		
		LifeSpan	50
		
		End

End

Condition
	On 	Time
	Time 	4

	Event
		
		Type	Local
		At	Hips
		
		Part3	:ShockFieldArchs2.part
		Part4	:ShockFieldArchs1.part
		Part5	:ShockFieldArchs.part
		LifeSpan	50
	End


End


#############################################################################
##  Blast
#######################################################

Condition
	On 	Time
	Time 	50

	Event
		
		Type	Local
		At	Hips
		Part1	:ShockFieldRing.part
		
		LifeSpan	27

	End

	Event
		
		Type	Local
		At	Hips
		Part2	:ShockFieldGlow.part
		
		LifeSpan	18
		
		End

End

Condition
	On 	Time
	Time 	54

	Event
		
		Type	Local
		At	Hips
		
		Part3	:ShockFieldArchs2.part
		Part4	:ShockFieldArchs1.part
		Part5	:ShockFieldArchs.part
			Lifespan 	54
	End


End





End
