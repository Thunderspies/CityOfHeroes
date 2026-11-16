#############################################################
## ElectricityBall.fx
#############################################################

FxInfo

LifeSpan 300

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
		Sound	EC_JoltingChain_Activate_01 100 100 1
		Lifespan 45
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
		Lifespan 45
	End
End

Condition
	On	Time
	Time	28


	Event
		EName	WepAnchor
		Type	Local
		At	HandL
		Part	:Hand_Ring_Burst.part
		Part	:WitchSparks.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 50.0 -10.0
		End
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Hand_Bolt_Blur_Small.part
		LookAt	HandL
		Pother	HandL
	End
End

Condition
	On	Time
	Time	32

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 50.0 -10.0
		End
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Hand_Bolt_Blur_Small.part
		LookAt	HandR
		Pother	HandR
	End
End

Condition
	On	Time
	Time	38

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 50.0 -10.0
		End
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Hand_Bolt_Blur_Small.part
		LookAt	HandL
		Pother	HandL
	End
End




#############################################################

Condition
	On 	Time
	Time 	75

	Event
		Ename	HandArcRings
		Type	Start
		At	HandL
		part	:Electric_Ring_Arcs.part
		Lifespan 5
	End


	Event
		EName 	Prime
		Type	start
		At	HandL
		Bhvr	:Medianprojectile1.bhvr
		Part1	:ElectricityControl01a.part
		Part3	:ElectricityInnerGlowa.part
		Part5	:ElectricityBallTail.part
		Sound elecball2 80 80 .8
		Magnet	Target
		Lookat	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End