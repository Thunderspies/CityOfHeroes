#############################################################
## ElectricityBall.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time	42
	Event
		Type	Local
		At	Root
		Sound 	ToV_BallLightning_Attack_01 100 100 .6
	End
End


Condition
	On	Time
	Time	42

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 50.0 -30.0
		End
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Hand_Bolt_Blur_Small.part
		LookAt	WepL
		Pother	WepL
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 50.0 -30.0
		End
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Hand_Bolt_Blur_Small.part
		LookAt	WepR
		Pother	WepR
	End
End

Condition
	On	Time
	Time	56

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 50.0 -30.0
		End
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Hand_Bolt_Blur_Small.part
		LookAt	WepL
		Pother	WepL
	End
End


#############################################################

Condition
	On 	Time
	Time 	42

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 30
	End


	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	42

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		Part	:Hand_Sparks.part
		Part	:Hand_Sparks_Long.part
		LifeSpan 15
	End
End

Condition
	On 	Time
	Time 	44

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan 15
	End
End

#####

Condition
	On 	Time
	Time 	42

	Event
		EName 	Thingy1
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 30
	End


	Event
		EName 	ThingyB
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	42

	Event
		EName 	BurstL
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		Part	:Hand_Sparks.part
		Part	:Hand_Sparks_Long.part
		LifeSpan 15
	End
End

Condition
	On 	Time
	Time 	44

	Event
		EName 	BurstL
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		LifeSpan 15
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
		Bhvr	:Medianprojectile1.bhvr #Behaviors/Braz/Projectile_Fast_01.bhvr
		Part1	:ElectricityControl01a.part
		Part3	:ElectricityInnerGlowa.part
		Part5	:ElectricityBallTail.part
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

	Event
		Type	Local
		At	Target
		Part	:Hit_Flash.part
		Part1	:ShockFieldArchsa.part
		Part2	:ShockFieldArchs2a.part
		Part3	:ShockFieldArchs1a.part
		part	:Electric_Ring_Arcs.part
		lifespan 30
	End

	Event
		ENAME	H
		Type	start
		At	T_Root
		geom	roottochestadjustment
		lifespan 30
	End

	Event
		Type	start
		At	H
		altpiv	1
		Part1	:ElectricitySparkBurst.part
		Part2	:ElectricitySparkBurst.part
		lifespan 30
	End

	Event
		Type	Local
		At	Target
		Part2	:ShockFieldGlow.part
		lifespan 5
	End

	Event
		Type	Local
		At	Target
		Sound 	ToV_BallLightning_Hit_01 100 100 .8
	End
End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End

#############################################################

End