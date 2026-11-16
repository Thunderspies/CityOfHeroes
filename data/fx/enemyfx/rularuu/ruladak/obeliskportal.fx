#########################################################
##	Fireweapon
########################################################
FxInfo


#LifeSpan	70
######################################################################

Condition

	Event
		EName 	Mallet1
		Type	start 
		At		root

		bhvrOverride			
			PositionOffset	0 15 0
		End
		
		Part	:StreaksThin.part
		Part	:Streaks.part
		Sound fury1_loop 200.0 200.0 .8
	End

	Event
		EName 	Mallet2
		Type	start 
		At		root
		bhvrOverride			
			PositionOffset	0 11 0
		End
		Part	:PortalRing.part
		Part	:PortalRingScalling.part
		Part	:StreaksThin1.part
		Part	:Streaks1.part

		Sound glow10b_loop 200.0 200.0 .6
	End
End

Condition
	On	Cycle
	Time	10	

	Event
		Type	start 
		At		Mallet2

		Part	:RuladakHandClapHitEmber.part
		Part	:RuladakHandClapHitStar.part
		Part	:RuladakHandClapHitRing.part
		Part	:RuladakHandClapHitGlows2.part
		LifeSpan	30
	End

End

Condition
	On	Cycle
	Time	18	

	Event
		Type	start 
		At		Mallet2
		Geom	SuperStrengthBubble

		BHVROVERRIDE
			Alpha	200
		End

		bhvr	behaviors/ScaleBubble.bhvr
		LifeSpan	7	
	End
End


End
##################################

