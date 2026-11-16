#############################################################
## Judgment_Ion.fx
#############################################################

FxInfo
LifeSpan	150

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Root
		Sound 	ToV_LightningStorm_Attack_01 100 100 .6
	End

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
	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Body_Electricity_Arcs.part

		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Sparks.part
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


#############################################################

End