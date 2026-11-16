#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	Head
End


#########################################################

Condition
	On 	Time
	Time 	36
	
	Event
		EName	Point
		Type	local	
		At	wepR
	
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Geom	heavyMace
	End	

	Event
		Type	local
		At	Point
		Altpiv	2
	
		#BHVR	V_COV/Weapons/maceBlastEmissionPoint.bhvr
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EmissionPoint.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BlastStreaks.part
		Sound BaneMaceScatterShot 100 100 .98
		LifeSpan	2
	End	

	Event
		EName	Prime
		Type	start
		At	Point
		Altpiv	2

		Bhvr	:Projectile1.bhvr
		Magnet	Target
		LookAt	Target
		
		
		Part	:SmallEnergyHead.part
		Part	:SmallEnergyHeadInvert.part
		Part	:SmallRapidEnergyCoreLocal.part
		Part	:SmallRapidEnergyCoreGlow.part

		Part	:SmallBotBubblesTrail.part
		Part	:SmallBotBallzTrail.part
		Part	:SmallBotTrailSpecks.part
		Part	:SmallBotTrail.part
		
	End

	
End

Condition
	On 	Time
	Time 	37
	
	Event
		EName	Prime2
		Type	Start
		At	Point
		Altpiv	2

		Bhvr	:Projectile1.bhvr
		BhvrOverride
			TrackRate	0.01
		End	
		Magnet	Target
		LookAt	Target

	End
End
Condition
	On 	Time
	Time 	37
	Repeat	6
	

	Event
		EName	Ancillary
		Type	Local
		At	Prime2

		Bhvr	behaviors/Quillprojectile2.bhvr
		LookAt	Target
		BhvrOverride
			InitialVelocity		0 0 6.25
			InitialVelocityJitter	2 .25 .75
		End		
		
		Part	:SmallEnergyHead.part
		Part	:SmallEnergyHeadInvert.part
		Part	:SmallRapidEnergyCoreLocal.part
		Part	:SmallRapidEnergyCoreGlow.part

		Part	:SmallBotBubblesTrail.part
		Part	:SmallBotBallzTrail.part
		Part	:SmallBotTrailSpecks.part
		Part	:SmallBotTrail.part

		Lifespan	30
		
	End

	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

End				