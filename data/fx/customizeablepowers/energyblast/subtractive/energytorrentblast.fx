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
		bhvr	:EnergytorrentScale.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		200
			PhysForcePowerJitter	50
			TintGeom		1
		End
		geom	Subtractive_EWave
		Sound energytorrent 80 80 0.95
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
		Part1	:CenterFlarePhat.part
		Part1	:CenterFlarePhat_Alpha.part

		Part2	:TorrentRaysCenter.part

	End

	Event
		Type	local
		At	Prime
		Altpiv	2
		Part1	:TorrentRays.part
		Part2	:EnergyTorrentStreak.part
		Part2	:EnergyTorrentStreak_Alpha.part

		Part4	:TorrentenergyBubbles.part
		Part3	:CometBubbles.part
		Part5	:CometBubblesWhite.part
		Part5	:CometBubblesWhite_Alpha.part
		Part3	:CometBubblesWhite2.part
	End

	Event
		Type	local
		At	Prime
		Altpiv	3
		Part1	:TorrentRays.part
		Part2	:EnergyTorrentStreak.part
		Part2	:EnergyTorrentStreak_Alpha.part

		Part4	:TorrentenergyBubbles.part
		Part3	:CometBubbles.part
		Part5	:CometBubblesWhite.part
		Part5	:CometBubblesWhite_Alpha.part

		Part3	:CometBubblesWhite2.part
	End

	Event
		Type	local
		At	Prime
		Altpiv	4
		Part1	:TorrentRays.part
		Part2	:EnergyTorrentStreak.part
		Part2	:EnergyTorrentStreak_Alpha.part

		Part4	:TorrentenergyBubbles.part
		Part3	:CometBubbles.part
		Part5	:CometBubblesWhite.part
		Part5	:CometBubblesWhite_Alpha.part

		Part3	:CometBubblesWhite2.part
	End

	Event
		EName	core
		Type	local
		At	root
		bhvr	:EnergytorrentScale2.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Subtractive_EWave
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