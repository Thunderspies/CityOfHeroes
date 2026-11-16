#############################################################
## Judgment_Ion.fx
#############################################################

FxInfo
LifeSpan	80

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part3	:Hand_Ring.part
		Part	:Hand_Sparks.part
		POther	LArmR
		Sound	ToV_BallLightning_Attack_01 100 100 .6
	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Body_Electricity_Arcs.part

		Part	:Hand_Glow.part
		Part3	:Hand_Ring.part
		Part	:Hand_Sparks.part
	End
End

Condition
	On 	Time
	Time	31
	Event
		Type	Local
		At	T_Chest
		Sound 	ToV_LightningBolt_Hit_01 100 100 .8
	End
End



Condition
	On	Time
	Time	28


	Event
		EName	WepAnchor
		Type	Local
		At	WepL
		Part	:Hand_Ring_Burst.part
		Part	:WitchSparks.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	31

	Event
		Ename	Offset01
		Type	Start
		At	T_Chest
		BhvrOverride
			Startjitter	8 0 8
			PositionOffset	0 30 0
		End
	End

	Event
		Ename	Offset02
		Type	Start
		At	T_Chest
		BhvrOverride
			Startjitter	6 2 6
			PositionOffset	0 15 0
		End
	End

	Event
		Ename	Offset03
		Type	Local
		At	T_Chest

	End

#############################################################

End