#########################################################
##	chill touch hit
FxInfo

LifeSpan	150

##################################

Condition
	On	Time
	Time	3
	
	Event
		EName	Ring
		Type	Start
		At	Hips
		Sound Powerup2 70 70 .8
	End
End

#######################################################################################
#################################  start growing glow  ################################
#######################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	1
		Type	Local
		At	WepL
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows.part
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows2.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		PMagnet ElbowL
		Lifespan 35

	End
End

#############################

Condition
	On	Time
	Time	2

	Event
		EName	2
		Type	Local
		At	HandL
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows.part
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows2.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		PMagnet HandL
		Lifespan 35
	End
End

Condition
	On	Time
	Time	5

	Event
		EName	2
		Type	Local
		At	LArmL
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows.part
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows2.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		PMagnet HandL
		Lifespan 35
	End
End

#############################

Condition
	On	Time
	Time	8

	Event
		EName	3
		Type	Local
		At	UArmL
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows.part
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows2.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		Lifespan 35
	End
End

#############################

Condition	
	On	Time
	Time	9

	Event
		EName	4
		Type	Local
		At	Hair
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EHeadGlows.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		Lifespan 35
	End

	Event
		EName	4a
		Type	Local
		At	Chest
		part1	V_COV\Powers\Paramilitary\Paramilitary_P_BoostBigGlowY.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		Lifespan 40
	End

	Event
		EName	5
		Type	Local
		At	Neck
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EHeadGlows.part
		Lifespan 35	
	End
End

#############################

Condition	
	On	Time
	Time	12

	Event
		EName	6
		Type	Local
		At	UArmR
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows.part
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows2.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		PMagnet ElbowL
		Lifespan 28
	End

	Event
		EName	7
		Type	Local
		At	Hips
#		part1	V_COV\Powers\Paramilitary\Paramilitary_P_BoostBigGlowY.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		Lifespan 28
	End
End

#############################

Condition	
	On	Time
	Time	15

	Event
		EName	8
		Type	Local
		At	LArmR
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows.part
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows2.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		PMagnet HandL
		Lifespan 23
	End

	Event
		EName	9
		Type	Local
		At	ULEGR
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows3.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		PMagnet Kneer
		Lifespan 23
	End

	Event
		EName	10
		Type	Local
		At	ULEGL
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows3.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		PMagnet Knee1
		Lifespan 23

	End

End

#############################

Condition	
	On	Time
	Time	18

	Event
		EName	11
		Type	Local
		At	HandR
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows.part
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows2.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		Lifespan 18
	End

	Event
		EName	12
		Type	Local
		At	LLEGL
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows3.part
#		part1	V_COV\Powers\Paramilitary\Paramilitary_P_BoostBigGlowY.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		PMagnet FootL
		Lifespan 18
	End

	Event
		EName	13
		Type	Local
		At	LLEGR
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows3.part
#		part1	V_COV\Powers\Paramilitary\Paramilitary_P_BoostBigGlowY.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		PMagnet FootR
		Lifespan 18
	End
End

#############################

Condition
	On	Time
	Time	21
	
	Event
		EName	14
		Type	Local
		At	FootR
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows4.part
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows4Flat.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		Lifespan 12
	End

	Event
		EName	13		
		Type	Local
		At	FootL
		part1	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows4.part
		part2	V_COV\Powers\Paramilitary\ParaMilitary_P_EGlows4Flat.part
		part3	V_COV\Powers\Paramilitary\Paramilitary_P_Specks2.part
		Lifespan 12
	End
End


#############################


End				