#############################################################
## Continuing_Plasma_Trail.fx
#############################################################

FxInfo
Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
		Sound eleccage_loop 70 70 .25
	End

End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	UarmR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	FootR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part

		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part

		Pmagnet FootL
	End


	Event
		Type	Local
		At	UarmL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet UarmL
	End


	Event
		ENAME	NeckNode
		Type	Local
		At	Neck
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:BodySmokeTrail01.part
		part	:BodySmokeTrailAdd.part
		part	:FireEmbersBody01.part
		Pmagnet NeckNode
		POther  NeckNode

	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	HipsNode
		Type	Local
		At	Hips

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet HipsNode
		POther  HipsNode

	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	SpadLNode
		Type	Local
		At	UArmL


	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet SpadLNode
		POther  SpadLNode

	End

End


Condition
	On	Time
	Time	0


	Event
		ENAME	SpadRNode
		Type	Local
		At	UarmR
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbersBody01.part
		Pmagnet SpadRNode
		POther  SpadRNode



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	ChestNode
		Type	Local
		At	Chest

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet ChestNode
		POther  ChestNode



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	LarmRNode
		Type	Local
		At	LarmR
		part	:BodySmokeTrailSmall01.part
		part	:BodySmokeTrailSmallAdd.part



	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LarmRNode
		POther  LarmRNode



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	LarmRNode2
		Type	Local
		At	LarmR

	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet LarmRnode2
		POther  LarmRNode2



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL
		part	:BodySmokeTrailSmall01.part
		part	:BodySmokeTrailSmallAdd.part



	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LarmLNode
		POther  LarmLNode



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	LarmLNode2
		Type	Local
		At	LarmL

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part

		Pmagnet LarmLNode2
		POther  LarmLNode2



	End
End

Condition
	On	Time
	Time	0


	Event
		ENAME	LleglNode
		Type	Local
		At	Llegl

	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LleglNode
		POther  LleglNode



	End
End



Condition
	On	Time
	Time	0

	Event
		ENAME	LlegRNode
		Type	Local
		At	LlegR
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LlegRNode
		POther  LlegRNode

	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	LlegR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:BodySmokeTrailSmall01.part
		part	:BodySmokeTrailSmallAdd.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:BodySmokeTrailSmall01.part
		part	:BodySmokeTrailSmallAdd.part
		Pmagnet FootL
		POther  FootL
	End

End



End