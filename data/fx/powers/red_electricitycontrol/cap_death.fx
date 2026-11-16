#########################################################
##	forceBubble
########################################################
FxInfo


LifeSpan	55

########################################################
Condition
	On 	triggerbits
	Triggerbits	death

	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/vanish.bhvr
	###	Flags	AdoptParentEntity
	End

	Event
		EName 	Prime
		Type	Destroy
	End
End
#############################################################################
##  Blast
#######################################################


Condition
	On 	triggerbits
	Triggerbits	death

	Event
		
		Type	Local
		At	Hips
		Sound elecfry 60 60 .43
	End

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
