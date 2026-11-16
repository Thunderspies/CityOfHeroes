
#########################################################################################
########	Notes: Recluse Mele attack with streaking from arms
########                           
#########################################################################################

FxInfo
	
LifeSpan	90

##################################################################################
###########################  Streaks for left arm ######################  01  ####
##################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	Child_HandR
		Type	Local
		At	HandR
		bhvrOverride
			PositionOffset	.25 0 0.02
		End
		ChildFX :F_Glow_RecluseRedHand_KnockUp.fx
		Lifespan 85
	End
End


Condition
	On	Time
	Time	33
	
	Event
		EName	Right01
		Type	Local
		At	Chest
		
		ChildFX V_COV\EnemyPowers\ArchVillains\Lord_Recluse\F_ChildStreak_RightKnockUp.fx
	End
End


End