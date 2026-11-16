#############################################################
## EnergyTorrentBlastRed.fx
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
		bhvr	behaviors\EnergytorrentScale.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		geom	EWaveRED
		Magnet	T_Root
		LookAt	T_Root
		Sound energytorrent 80 80 .95
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
		EName	alt1
		Type	local
		At	Prime
		Altpiv	1

		Part1	:CenterFlarePhatRED.part
		Part2	:TorrentRaysCenterRED.part
		#Part3	:EnergyTorrentStreak.part
	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	2

		Part1	:TorrentRaysRED.part
		Part2	:EnergyTorrentStreakRED.part

		Part3	:REDCometBubbles.part
		Part4	:TorrentenergyBubblesRED.part
		Part5	:CometBubblesWhiteRED.part
	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	3

		Part1	:TorrentRaysRED.part
		Part2	:EnergyTorrentStreakRED.part

		Part3	:REDCometBubbles.part
		Part4	:TorrentenergyBubblesRED.part
		Part5	:CometBubblesWhiteRED.part
	End


	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	2


		Part3	:CometBubblesWhite2RED.part

	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	3



		Part3	:CometBubblesWhite2RED.part

	End


	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	4

		Part1	:CenterFlareRED.part
		Part2	:TorrentRaysCenterRED.part
		Part3	:EnergyTorrentStreakRED.part

	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	5

		Part1	:CenterFlareRED.part
		Part2	:TorrentRaysCenterRED.part
		Part3	:EnergyTorrentStreakRED.part
	End

	Event
		EName	core
		Type	local
		At	root
		geom	EWaveRED
		bhvr	behaviors\EnergytorrentScale2.bhvr
		#Sound	PowerBolt4 100.0 30.0 .7
		LifeSpan		4
	End

End





###################################################################




Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy


	End

	Event
		EName 	cometParts
		Type	Destroy


	End

End


End