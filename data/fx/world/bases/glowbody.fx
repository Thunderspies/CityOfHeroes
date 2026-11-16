#########################################################
##	template

FxInfo

Lifespan 80
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
		ENAME	HeadNode
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
	End

	Event
		Type	Local
		At	HeadNode
		part	WORLD\Bases\Glow.part
		Pmagnet Head
	End

	Event
		Type	Local
		At	UarmR
		part	WORLD\Bases\Glow.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	FootR
		part	WORLD\Bases\Glow.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	WORLD\Bases\Glow.part
		Pmagnet FootL
	End


	Event
		Type	Local
		At	UarmL
		part	WORLD\Bases\Glow.part
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
		part	WORLD\Bases\Glow.part
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
		part	WORLD\Bases\Glow.part
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
		part	WORLD\Bases\Glow.part
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
		part	WORLD\Bases\Glow.part
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
		At	Head
		part	WORLD\Bases\Glow.part
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

	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	WORLD\Bases\Glow.part
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
		part	WORLD\Bases\Glow.part
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

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	WORLD\Bases\Glow.part
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
		part	WORLD\Bases\Glow.part
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
		part	WORLD\Bases\Glow.part
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
		part	WORLD\Bases\Glow.part
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
		part	WORLD\Bases\Glow.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	WORLD\Bases\Glow.part
		Pmagnet FootL
		POther  FootL
	End

End



End