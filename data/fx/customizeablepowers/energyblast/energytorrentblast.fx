#############################################################
## EnergyTorrentBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	22

	Event
		EName	Prime
		Type	start
		At	root
		bhvr	CustomizeablePowers\EnergyBlast\EnergytorrentScale.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		200
			PhysForcePowerJitter	50
			TintGeom		1
		End
		geom	tintable_EWave
		Sound energytorrent 80 80 .95
		Magnet	T_Root
		LookAt	T_Root
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime
		BhvrOverride
			PhysForceType		Forward
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
	End

	Event
		Type	local
		At	Prime
		Altpiv	1
		Part1	CustomizeablePowers\EnergyBlast\CenterFlarePhat.part
		Part2	CustomizeablePowers\EnergyBlast\TorrentRaysCenter.part
	End

	Event
		Type	local
		At	Prime
		Altpiv	2
		Part1	CustomizeablePowers\EnergyBlast\TorrentRays.part
		Part2	CustomizeablePowers\EnergyBlast\EnergyTorrentStreak.part
		Part4	CustomizeablePowers\EnergyBlast\TorrentenergyBubbles.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part5	CustomizeablePowers\EnergyBlast\CometBubblesWhite.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubblesWhite2.part
	End

	Event
		Type	local
		At	Prime
		Altpiv	3
		Part1	CustomizeablePowers\EnergyBlast\TorrentRays.part
		Part2	CustomizeablePowers\EnergyBlast\EnergyTorrentStreak.part
		Part4	CustomizeablePowers\EnergyBlast\TorrentenergyBubbles.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part5	CustomizeablePowers\EnergyBlast\CometBubblesWhite.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubblesWhite2.part
	End

	Event
		Type	local
		At	Prime
		Altpiv	4
		Part1	CustomizeablePowers\EnergyBlast\TorrentRays.part
		Part2	CustomizeablePowers\EnergyBlast\EnergyTorrentStreak.part
		Part4	CustomizeablePowers\EnergyBlast\TorrentenergyBubbles.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part5	CustomizeablePowers\EnergyBlast\CometBubblesWhite.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubblesWhite2.part
	End

	Event
		EName	core
		Type	local
		At	root
		bhvr	CustomizeablePowers\EnergyBlast\EnergytorrentScale2.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	tintable_EWave
		LifeSpan 4
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