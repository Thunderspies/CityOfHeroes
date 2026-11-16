#############################################################
## FollowUpMaceHit.fx
#############################################################

FxInfo

LifeSpan	75


## GLOWS ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		BhvrOverride
			PositionOffset	0 0 .7
		End
		Part	:HandleGlowsTiny.part
		Part	:HandleGlow.part
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		LifeSpan	30
	End
	
	Event
		EName 	Prime
		Type	Local 
		At	WepR
		BhvrOverride
			PositionOffset	0 0 2.1
		End
		Part	:MaceBallGlowTiny.part
		Part	:MaceBallGlow.part
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		LifeSpan	30
	End
End

#############################################################

Condition
	On	Time 
	Time	15 #0
	
	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	:FollowUpGlow.part
		Part2	:FollowUpSpecks2.part
		Sound clawfollow3 100 100 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

Condition
	On	Time 
	Time	25 #10
	
#	Event
#		EName 	Prime
#		Type	Local 
#		At	WepR
#		Geom	GEO_WepR_Mace_1
#		Bhvr	Behaviors/Weapon_Fade_UP.bhvr	
#	End

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part2	:FollowUpRing01.part
		Part3	:FollowUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

Condition
	On	Time 
	Time	35 #20
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part3	:FollowUpRing02.part
		Part4	:FollowUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

Condition
	On	Time 
	Time	30 #45
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks2.part
		Part2	:FollowUpRingSlow.part
		Part3	:FollowUpRing03.part
		Part4	:FollowUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End
End

## CLEAN UP ###########################################################

Condition
	On	Time 
	Time	28 #13

	Event
		EName	pow2
		Type 	Destroy
	End
End

Condition
	On	Time 
	Time	75

	Event
		EName	all
		Type 	Destroy
	End
End

#############################################################
	
End