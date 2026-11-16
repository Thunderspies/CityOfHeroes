#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	200

###########################################################


Condition
	On	Time
	Time	22
	
	Event
		EName	Prime
		Type	start
		At	root
		geom	WhiteEWave
		bhvr	behaviors/EnergytorrentScale.bhvr
		bhvrOverride
			StartColor	150 75 255
		End
		Magnet T_Root
		LookAt T_Root
		Sound energytorrent 80 80 .95
	
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
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

		Part1	:CenterFlarePhat.part
		Part2	:TorrentRaysCenter.part
		#Part3	:EnergyTorrentStreak.part
	End
	
	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	2

		Part1	:TorrentRays.part
		Part2	:EnergyTorrentStreak.part

		Part3	:CometBubbles.part
		Part4	:TorrentenergyBubbles.part
		Part5	:CometBubblesWhite.part
	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	3

		Part1	:TorrentRays.part
		Part2	:EnergyTorrentStreak.part
		
		Part3	:CometBubbles.part
		Part4	:TorrentenergyBubbles.part
		Part5	:CometBubblesWhite.part
	End


	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	2

		
		Part3	:CometBubblesWhite2.part
		
	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	3

		
		
		Part3	:CometBubblesWhite2.part
		
	End


	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	4

		Part1	:CenterFlare.part
		Part2	:TorrentRaysCenter.part
		Part3	:EnergyTorrentStreak.part
		
	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	5

		Part1	:CenterFlare.part
		Part2	:TorrentRaysCenter.part
		Part3	:EnergyTorrentStreak.part
	End

	Event
		EName	core
		Type	local
		At	root
		geom	WhiteEWave
		bhvr	behaviors/EnergytorrentScale2.bhvr
		bhvrOverride
			StartColor	150 75 255
		End
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