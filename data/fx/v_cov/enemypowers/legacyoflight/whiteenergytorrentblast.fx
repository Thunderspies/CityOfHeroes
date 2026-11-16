#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
Input  
	InpName	Origin
End


Input  
	InpName	Target
End


Input  
	InpName	mystic
End

Input  
	InpName	Root
End

Input  
	InpName	T_Root
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

LifeSpan	200

###########################################################




Condition

	On	Time
	Time	10
	
	Event
		Type	local
		At	root
		Sound lighttorrent2 90 90 1.0
	End

	On	Time
	Time	22
	
	Event
		EName	Prime
		Type	start
		At	root
		geom	WhiteEWave
		bhvr	behaviors/WhiteEnergytorrentScale.bhvr
		
		Magnet T_Root
		LookAt T_Root
		#Sound	lighttorrent2 80 30 1.0
	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	1

		Part1	:WhiteCenterFlarePhat.part
		Part2	:WhiteTorrentRaysCenter.part
		#Part3	:WhiteEnergyTorrentStreak.part
	End
	
	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	2

		Part1	:WhiteTorrentRays.part
		Part2	:WhiteEnergyTorrentStreak.part

		Part3	:WhiteCometBubbles.part
		Part4	:WhiteTorrentenergyBubbles.part
		Part5	:WhiteCometBubblesWhite.part
	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	3

		Part1	:WhiteTorrentRays.part
		Part2	:WhiteEnergyTorrentStreak.part
		
		Part3	:WhiteCometBubbles.part
		Part4	:WhiteTorrentenergyBubbles.part
		Part5	:WhiteCometBubblesWhite.part
	End


	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	2

		
		Part3	:WhiteCometBubblesWhite2.part
		
	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	3

		
		
		Part3	:WhiteCometBubblesWhite2.part
		
	End


	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	4

		Part1	:WhiteCenterFlare.part
		Part2	:WhiteTorrentRaysCenter.part
		Part3	:WhiteEnergyTorrentStreak.part
		
	End

	Event
		EName	alt1
		Type	local
		At	Prime
		Altpiv	5

		Part1	:WhiteCenterFlare.part
		Part2	:WhiteTorrentRaysCenter.part
		Part3	:WhiteEnergyTorrentStreak.part
	End

	Event
		EName	core
		Type	local
		At	root
		geom	WhiteEWave
		bhvr	behaviors/WhiteEnergytorrentScale2.bhvr
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