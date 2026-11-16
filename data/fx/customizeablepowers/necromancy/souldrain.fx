

#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	CustomizeablePowers\Necromancy\DarkBlastTenticle.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmberb.part
		Sound SoulDrain2 80 80 .86
				
		PMagnet	LarmL
		LifeSpan	50
	End
End

Condition
	On 	Time
	Time 	14

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	CustomizeablePowers\Necromancy\DarkBlastTenticle.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmberb.part
		
		PMagnet	WepL
		LifeSpan	42
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	Prime
		Type	start
		At	WepL

		Geom	Skull		
		Bhvr	CustomizeablePowers\Necromancy\Skullprojectile1.bhvr
		
		Magnet	Target
		LookAt  Target
		
	
	End
	
	Event
		EName	eye1
		Type	local
		At	Prime
		 altpiv	2
		Part1	CustomizeablePowers\Necromancy\DaemonEyes2.part
		Part2	CustomizeablePowers\Necromancy\DaemonPupil2.part
	End

	Event
		EName	eye2
		Type	local
		At	Prime
		 altpiv	3
		Part1	CustomizeablePowers\Necromancy\DaemonEyes2.part
		Part2	CustomizeablePowers\Necromancy\DaemonPupil2.part
	End

	Event
		EName	Prime2
		Type	start
		At	Prime
		 altpiv	1

		Part1	CustomizeablePowers\Necromancy\DarknessRapidTenticles.part
		Part2	CustomizeablePowers\Necromancy\DarknessRapidCloud.part
		Part3	CustomizeablePowers\Necromancy\DarknessRapidCloud2.part
		Part4	CustomizeablePowers\Necromancy\SoulDrainSkullGlow.part
		
		Bhvr	CustomizeablePowers\Necromancy\Skullprojectile1.bhvr
		
		Magnet	Target
		LookAt  Target
	
	End

End

Condition
	On 	PrimeHit

	Event
		EName	skullHitFake
		Type	local
		At	T_Chest
		
		Part1	CustomizeablePowers\Necromancy\SkullHit.part
		Part2	CustomizeablePowers\Necromancy\SkullPuddle.Part
		LifeSpan	25	
	End
	
	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy

	End
End


End	



			