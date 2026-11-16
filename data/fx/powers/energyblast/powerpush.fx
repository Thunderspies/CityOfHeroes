#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	16

	Event
		EName	core
		Type	local
		At	BendMystic
		geom	MegaCore
		bhvr	behaviors\BlastScale3.bhvr
		Sound Powerpush 100.0 100.0 .9
		Lifespan 26
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	WepL
		Part3	:CometHandStarWhite1.part
		Part4	:CometHandCloudsWhite1.part
		Part1	:PushBubblesWhite.part
		Part2	:PushBubbles.part
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
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3.part
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
		bhvr	behaviors\PushProjectile.bhvr
		geom	Push
		Part2	:CometBubblesWhiteCenter.part
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

	Event
		EName	hookup1
		Type	local
		At	Prime
		Altpiv	1
		Anim	FX_dark
		Bhvr	Behaviors\PushAnimScale.bhvr
	End

	Event
		Type	local
		At	hookup1
		AnimPiv	Chest
		Part1	:PowerPushStreak.part
		Part3	:CometBubbles.part
		Part4	:TorrentenergyBubbles2.part
		Part5	:CometBubblesWhite3.part
	End

	Event
		Type	local
		At	hookup1
		AnimPiv Neck
		Part1	:PowerPushStreak.part
		Part3	:CometBubbles.part
		Part4	:TorrentenergyBubbles2.part
		Part5	:CometBubblesWhite3.part
	End

	Event
		Type	local
		At	hookup1
		AnimPiv Waist
		Part1	:PowerPushStreak.part
		Part3	:CometBubbles.part
		Part4	:TorrentenergyBubbles2.part
		Part5	:CometBubblesWhite3.part
	End

	Event
		EName	hookup2
		Type	local
		At	Prime
		Altpiv	2
		Anim	FX_dark
		Bhvr	Behaviors\PushAnimScale.bhvr
	End

	Event
		Type	local
		At	hookup2
		AnimPiv Chest
		Part1	:PowerPushStreak.part
		Part3	:CometBubbles.part
		Part4	:TorrentenergyBubbles2.part
		Part5	:CometBubblesWhite3.part
	End

	Event
		Type	local
		At	hookup2
		AnimPiv Neck
		Part1	:PowerPushStreak.part
		Part3	:CometBubbles.part
		Part4	:TorrentenergyBubbles2.part
		Part5	:CometBubblesWhite3.part
	End

	Event
		Type	local
		At	hookup2
		AnimPiv Waist
		Part1	:PowerPushStreak.part
		Part3	:CometBubbles.part
		Part4	:TorrentenergyBubbles2.part
		Part5	:CometBubblesWhite3.part
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	core
		altpiv	1
		Part5	:EnergyCoreLocal.part
		Part1	:CoreEnergy.part
		Part2	:CoreEnergyBeam.part
		Lifespan 30
	End

	Event
		EName	cometBalls
		Type	Local
		At	core
		altpiv	1
		Part3	:CometBubbles2.part
		Part4	:CometBubbles3.part
		Lifespan 10
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