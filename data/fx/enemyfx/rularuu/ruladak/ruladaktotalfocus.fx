#########################################################
##	Fireweapon
########################################################
FxInfo


LifeSpan	70
######################################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet1
		Type	local 
		At	Chest
		Geom	SuperStrengthBubble
		bhvr	behaviors/ScaleBubble.bhvr
		LifeSpan	7	
	End

	Event
		EName 	Mallet2
		Type	local 
		At	Chest
		#bhvr	behaviors/PivotOffsetChest.bhvr
		Part	:RuladakHandClapHitEmber.part
		Part	:RuladakHandClapHitStar.part
		Part	:RuladakHandClapHitRing.part
		Part	:RuladakHandClapHitRing2.part
		Part	:RuladakHandClapHitGlows2.part
	
		Sound PunchHit2 100.0 100.0 .8
	End

	Event
		EName 	Mallet2
		Type	local 
		At	Chest
		#bhvr	behaviors/PivotOffsetChest.bhvr
		
		Part	:ChestHitStreaksThin.part
		Part	:ChestHitStreaks.part
		LifeSpan	10
	End

	Event
		EName 	Mallet2
		Type	local 
		At	Chest
		#bhvr	behaviors/PivotOffsetChest.bhvr
		
		Part	:BubbleChestDots.part
		
		LifeSpan	15
	End

End


End
##################################

