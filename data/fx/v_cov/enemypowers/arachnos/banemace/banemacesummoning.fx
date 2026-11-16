#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	90

#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName	Point
		Type	local	
		At	wepR
	
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Geom	heavyMace
		
	End	

End

Condition
	On 	Time
	Time 	6
	
	Event
		EName	Prime
		Type	Local
		At	Point
		Altpiv	2

		#Bhvr	:Projectile1.bhvr
		Magnet	Prime
		LookAt	Prime
		
		
		#Part	:EnergyHeadBuildUP.part
		Part	:RapidCoreLocal.part
		Part	:RapidCoreLocalInvert.part
		
		#Part	:BotTrailBuildUP.part
		Part	:BotTrailSpecksBuildUP.part
		#Part	:BotBubblesBuildUP.part
		Part	:BotBallzSparkler.part
		Sound BaneMaceSummon2 100 100 .95
		
	End

	
End

Condition
	On 	Time
	Time 	36
	
	Event
		EName	Flash
		Type	Local
		At	Point
		Altpiv	2

		Magnet	Prime
		LookAt	Prime
		
		Part	:RapidEnergyCoreLocal.part
		Part	:SmallRapidEnergyCoreGlow.part
		Part	:SummonPulseFlash.part
		Part	:SummonPulseRing.part

		Lifespan	24
			
	End
	
End

End				