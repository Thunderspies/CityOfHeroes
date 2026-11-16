#############################################################
## PowerPush.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	WepL
		Part3	CustomizeablePowers\EnergyBlast\CometHandStarWhite1.part
		Part4	CustomizeablePowers\EnergyBlast\CometHandCloudsWhite1.part
		Part1	CustomizeablePowers\EnergyBlast\PushBubblesWhite.part
		Part2	CustomizeablePowers\EnergyBlast\PushBubbles.part
		Lifespan 47
	End

	Event
		Type	local
		At	WepR
		Part3	CustomizeablePowers\EnergyBlast\CometHandStarWhite1.part
		Part4	CustomizeablePowers\EnergyBlast\CometHandCloudsWhite1.part
		Part1	CustomizeablePowers\EnergyBlast\PushBubblesWhite.part
		Part2	CustomizeablePowers\EnergyBlast\PushBubbles.part
		Lifespan 47
	End
End

Condition
	On 	Time
	Time 	37

	Event
		EName 	HandGlowFlashL
		Type	local
		At	WepL
		Part1	CustomizeablePowers\EnergyBlast\HandGlowWhite3.part
		Part2	CustomizeablePowers\EnergyBlast\HandGlowEmber3.part
	End

	Event
		EName 	HandGlowFlashL
		Type	local
		At	WepR
		Part1	CustomizeablePowers\EnergyBlast\HandGlowWhite3.part
		Part2	CustomizeablePowers\EnergyBlast\HandGlowEmber3.part
	End
End

#############################################################

Condition
	On	Time
	Time	16

	Event
		EName	core
		Type	local
		At	BendMystic
		Bhvr	CustomizeablePowers\EnergyBlast\BlastScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_MegaCore
		Sound Powerpush 100.0 100.0 .9
		Lifespan 26
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	core
		altpiv	1
		Part5	CustomizeablePowers\EnergyBlast\EnergyCoreLocal.part
		Part1	CustomizeablePowers\EnergyBlast\CoreEnergy.part
		Part2	CustomizeablePowers\EnergyBlast\CoreEnergyBeam.part
		Lifespan 30
	End

	Event
		Type	Local
		At	core
		altpiv	1
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles2.part
		Part4	CustomizeablePowers\EnergyBlast\CometBubbles3.part
		Lifespan 10
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName	Prime
		Type	start
		At	BendMystic
		Bhvr	Behaviors\PushProjectile.Bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Push
		Part2	CustomizeablePowers\EnergyBlast\CometBubblesWhiteCenter.part
		Magnet	Target
		LookAt  Target
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime
		BhvrOverride
			pyrRotate		10 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		400
			PhysForcePowerJitter	100
		End
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName	hookup1
		Type	local
		At	Prime
		Altpiv	1
		Bhvr	CustomizeablePowers\EnergyBlast\PushAnimScale.Bhvr
		Anim	FX_dark
	End

	Event
		Type	local
		At	hookup1
		AnimPiv	Chest
		Part1	CustomizeablePowers\EnergyBlast\PowerPushStreak.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part4	CustomizeablePowers\EnergyBlast\TorrentenergyBubbles2.part
		Part5	CustomizeablePowers\EnergyBlast\CometBubblesWhite3.part
	End

	Event
		Type	local
		At	hookup1
		AnimPiv Neck
		Part1	CustomizeablePowers\EnergyBlast\PowerPushStreak.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part4	CustomizeablePowers\EnergyBlast\TorrentenergyBubbles2.part
		Part5	CustomizeablePowers\EnergyBlast\CometBubblesWhite3.part
	End

	Event
		Type	local
		At	hookup1
		AnimPiv Waist
		Part1	CustomizeablePowers\EnergyBlast\PowerPushStreak.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part4	CustomizeablePowers\EnergyBlast\TorrentenergyBubbles2.part
		Part5	CustomizeablePowers\EnergyBlast\CometBubblesWhite3.part
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	hookup2
		Type	local
		At	Prime
		Altpiv	2
		Anim	FX_dark
		Bhvr	CustomizeablePowers\EnergyBlast\PushAnimScale.Bhvr
	End

	Event
		Type	local
		At	hookup2
		AnimPiv Chest
		Part1	CustomizeablePowers\EnergyBlast\PowerPushStreak.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part4	CustomizeablePowers\EnergyBlast\TorrentenergyBubbles2.part
		Part5	CustomizeablePowers\EnergyBlast\CometBubblesWhite3.part
	End

	Event
		Type	local
		At	hookup2
		AnimPiv Neck
		Part1	CustomizeablePowers\EnergyBlast\PowerPushStreak.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part4	CustomizeablePowers\EnergyBlast\TorrentenergyBubbles2.part
		Part5	CustomizeablePowers\EnergyBlast\CometBubblesWhite3.part
	End

	Event
		Type	local
		At	hookup2
		AnimPiv Waist
		Part1	CustomizeablePowers\EnergyBlast\PowerPushStreak.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part4	CustomizeablePowers\EnergyBlast\TorrentenergyBubbles2.part
		Part5	CustomizeablePowers\EnergyBlast\CometBubblesWhite3.part
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	ALL #Prime
		Type	Destroy
	End
End

#############################################################

End