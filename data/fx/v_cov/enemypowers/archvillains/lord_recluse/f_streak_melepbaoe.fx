
#########################################################################################
########	Notes: Recluse Mele attack with streaking from arms
########                           
#########################################################################################

FxInfo
	
LifeSpan	125



Condition

	On	Time
	Time	7
	Event
		Type	Local
		At	Root
		Sound scorpionbuildup2 70 70 .77
	End

End


Condition

	On	Time
	Time	45
	Event
		Type	Local
		At	Root
		Sound reclusepunch2 140 140 .89
	End

End




##################################################################################
###########################  HANDS GLOW  #########################################
##################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	Child_HandR
		Type	Local
		At	HandR
		bhvrOverride
#			PositionOffset	.25 0 0.02
		End
		ChildFX :F_Glow_RecluseRedHand_PBAoE.fx
		Lifespan 105
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Child_HandL
		Type	Local
		At	HandL
		bhvrOverride
			PositionOffset	-.4 0 0.02
		End
		ChildFX :F_Glow_RecluseRedHand_PBAoE.fx
		Lifespan 115
	End
End

##############################################################################################
######  build up stretch from hand to elbowt    LOWER ARMs
##############################################################################################

Condition
	On	Time
	Time	7

	Event
		EName	Child_ForeArmL
		Type	Local
		At	LArmL
		bhvrOverride
			PositionOffset	0 0 0
		End

		ChildFX :F_ChildGlow_RedLArm_PBAoE.fx
		Lifespan 115
	End
End

Condition
	On	Time
	Time	7

	Event
		EName	Child_ForeArmL
		Type	Local
		At	LArmR
		bhvrOverride
			PositionOffset	0 0 0
			PYRRotate	0 180 0
		End

		ChildFX :F_ChildGlow_RedLArm_PBAoE.fx
		Lifespan 115
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	Child_Gather
		Type	Local
		At	HandR

		ChildFX :F_Gather_ReclusePBAoE.fx
	End
End

Condition
	On	Time
	Time	40

	Event
		EName	Child_Gather_Arms
		Type	Local
		At	HandR

		ChildFX :F_Glow_BackpackFinalBurst_PBAoE.fx
	End
End

##############################################################################################
######  STREAK FOR BACKPACK
##############################################################################################

Condition
	On	Time
	Time	40

	Event
		EName	Child_ForeArmL
		Type	Local
		At	LArmR
		bhvrOverride
			PositionOffset	0 0 0
		End

		ChildFX :F_ChildStreak_BackPackTipsR.fx
		Lifespan 115
	End
End

Condition
	On	Time
	Time	43

	Event
		EName	Child_ForeArmL
		Type	Local
		At	LArmR
		bhvrOverride
			PositionOffset	0 0 0
		End

		ChildFX :F_ChildStreak_BackPackTipsL.fx
		Lifespan 115
	End
End





End