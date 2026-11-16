#############################################################
## Vanguard Heavy - Plasma Blast
#############################################################

FxInfo

LifeSpan  400

#############################################################

Condition
	On	Time
	Time	13
	
	Event
		Type	Local
		At 	Chest
		Sound HeavyPlasmaBlast2 100 100 .9
	End
End

Condition
	On	Time
	Time	0
	
	Event
		EName	WepROffset
		Type	Local
		At 	LArmR
		BhvrOverride
			PositionOffset	2 0 0
		End
		#Geom	GEO_FX_TestSphere
	End

	Event
		EName	WepLOffset
		Type	Local
		At 	LArmL
		BhvrOverride
			PositionOffset	-2 0 0
		End
		#Geom	GEO_FX_TestSphere
	End
End

## CHARGE-UP ###########################################################

Condition
	On	Time
	Time	10
			
	Event
		Type	local
		At	WepROffset
#		Part	:EnergyLiquidFirst.part
#		Part	:EnergyLiquid.part
		Part	:NavyStreaksIn.part
		Part	:WhiteStreaksIn.part
		Magnet	Target
		Lookat	Target
		LifeSpan	40
	End
		
	Event
		Type	local
		At	WepROffset
		Part	:mainEnergyCore.part
		Part	:MainEnergyRing.part
		Part	:MainEnergyThinRing.part
		Magnet	Target
		Lookat	Target
		LifeSpan	40
	End
End

Condition
	On	Time
	Time	20
			
	Event
		Type	local
		At	WepLOffset
#		Part	:EnergyLiquidFirst.part
#		Part	:EnergyLiquid.part
		Part	:NavyStreaksIn.part
		Part	:WhiteStreaksIn.part
		Magnet	Target
		Lookat	Target
		LifeSpan	30
	End
		
	Event
		Type	local
		At	WepLOffset
		Part	:mainEnergyCore.part
		Part	:MainEnergyRing.part
		Part	:MainEnergyThinRing.part
		Magnet	Target
		Lookat	Target
		LifeSpan	30
	End
End

## BEAM - RIGHT ###########################################################

Condition
	On 	Time
	Time 	74

	Event
		Type	Local
		At	WepROffset
		Part	:Blast3.part
		Part	:Blast2.part
		LifeSpan	4
	End

	Event
		Type	Local
		At	WepROffset
		Part	:HeavyEnergyBlast.part
		Part	:HeavyEnergyBlastNavy.part
		Magnet	Target
		Lookat	Target
		LifeSpan	23
	End
End

Condition
	On 	Time
	Time 	76

	Event
		ENAME   prime
		Type	start 
		At	WepROffset
		Bhvr	:Projectile.bhvr
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
			#PositionOffset	0 -0.25 0
		End
		Magnet	Target
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		Bhvroverride
			TrackRate		25
		End
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part5	:EnergyCoreLocal.part
	End
End

Condition
	On 	Time
	Time 	78

	Event
		ENAME   prime1
		Type	start 
		At	WepROffset
		Bhvr	:Projectile.bhvr
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
			#PositionOffset	0 -0.25 0
		End
		Magnet	Target
	End

	Event
		Ename hook
		Type	local
		At	prime1
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1		
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
	
	Event
		Type	local
		At	prime1
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1		
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
End

Condition
	On 	Time
	Time 	82

	Event
		ENAME   prime2
		Type	start 
		At	WepROffset
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
			#PositionOffset	0 -0.25 0
		End
		Bhvr	:Projectile.bhvr
		Magnet	Target
	End

	Event
		Ename hook
		Type	local
		At	prime2
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
	
	Event
		Type	local
		At	prime2
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
End

Condition
	On 	Time
	Time 	85

	Event
		ENAME   prime3
		Type	start 
		At	WepROffset
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1
			#PositionOffset	0 -0.25 0
		End
		Bhvr	:Projectile.bhvr
		Magnet	Target
	End

	Event
		Ename hook
		Type	local
		At	prime3

		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1		
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
	
	Event
		Type	local
		At	prime3
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
End

Condition
	On 	Time
	Time 	76

	Event
		Ename	caustic
		Type	Start
		At	WepROffset	
		Part	:PlasmaTrail.part
		Magnet	Target
		Lookat	Target
		LifeSpan	20
	End

	Event
		Ename	Blasthead
		Type	Start
		At	WepROffset	
		#BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:HeavyVillianSniper_Tracer.part
		Part	:HeavyVillianSniper_Head.part
		Magnet	Target
		Lookat	Target
	End
	
	Event
		Ename hook
		Type	local
		At	prime
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1		
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
	
	Event
		Type	local
		At	prime
		Bhvroverride
		
			StartJitter	1 1 1
			Spin		4 1 1	
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End

	Event
		Type	Start
		At	WepROffset	
		Part	:HeavyEnergyBlast3.part
		Part	:HeavyEnergyBlast2.part
		Magnet	Target
		Lookat	Target
		LifeSpan	20
	End
	
	Event
		Type	Start
		At	WepROffset	
#		Bhvroverride
#			PositionOffset	0 -0.25 0
#		End	
		Part	:HeavyEnergyBlastHead2.part
		Magnet	Target
		Lookat	Target
		LifeSpan	18
	End

	Event
		Type	Start
		At	WepROffset	
#		Bhvroverride
#			PositionOffset	0 -0.25 0
#		End	
		Part	:HeavyEnergyBlastCore.part
		Part	:HeavyEnergyBlastCoreInvert.part
		Magnet	Target
		Lookat	Target
		LifeSpan	20
		LifeSpanJitter	7
	End

	Event
		Type	Start
		At	WepROffset	
#		Bhvroverride
#			PositionOffset	0 -0.25 0
#		End	
		Part	:HeavyEnergyBlastCoreWhite.part
		Magnet	Target
		Lookat	Target
		LifeSpan	26
		LifeSpanJitter	7
	End

	Event
		Type	Start
		At	WepROffset	
#		Bhvroverride
#			PositionOffset	0 -0.25 0
#		End	
		Part	:HeavyEnergyBlast.part
		Part	:HeavyEnergyBlastNavy.part
		Magnet	Target
		Lookat	Target
		LifeSpan	5

	End

	Event
		Type	Start
		At	WepROffset	
#		Bhvroverride
#			PositionOffset	0 -0.25 0
#		End	
		Part	:NavyStreaks.part
		Part	:whiteStreaks.part
		Magnet	Target
		Lookat	Target
		LifeSpan	10
	End
End

## BEAM - RIGHT ###########################################################

Condition
	On 	Time
	Time 	74

	Event
		Type	Local
		At	WepLOffset
		Part	:Blast3.part
		Part	:Blast2.part
		LifeSpan	4
	End

	Event
		Type	Local
		At	WepLOffset
		Part	:HeavyEnergyBlast.part
		Part	:HeavyEnergyBlastNavy.part
		Magnet	Target
		Lookat	Target
		LifeSpan	23
	End
End

Condition
	On 	Time
	Time 	76

	Event
		ENAME   prime
		Type	start 
		At	WepLOffset
		Bhvr	:Projectile.bhvr
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
			#PositionOffset	0 -0.25 0
		End
		Magnet	Target
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		Bhvroverride
			TrackRate		25
		End
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part5	:EnergyCoreLocal.part
	End
End

Condition
	On 	Time
	Time 	76

	Event
		Ename	caustic
		Type	Start
		At	WepLOffset	
		Part	:PlasmaTrail.part
		Magnet	Target
		Lookat	Target
		LifeSpan	20
	End

	Event
		Ename	Blasthead
		Type	Start
		At	WepLOffset	
		#BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:HeavyVillianSniper_Tracer.part
		Part	:HeavyVillianSniper_Head.part
		Magnet	Target
		Lookat	Target
	End
	
	Event
		Ename hook
		Type	local
		At	prime
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1		
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
	
	Event
		Type	local
		At	prime
		Bhvroverride
		
			StartJitter	1 1 1
			Spin		4 1 1	
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End

	Event
		Type	Start
		At	WepLOffset	
		Part	:HeavyEnergyBlast3.part
		Part	:HeavyEnergyBlast2.part
		Magnet	Target
		Lookat	Target
		LifeSpan	20
	End
	
	Event
		Type	Start
		At	WepLOffset	
		Part	:HeavyEnergyBlastHead2.part
		Magnet	Target
		Lookat	Target
		LifeSpan	18
	End

	Event
		Type	Start
		At	WepLOffset	
		Part	:HeavyEnergyBlastCore.part
		Part	:HeavyEnergyBlastCoreInvert.part
		Magnet	Target
		Lookat	Target
		LifeSpan	20
		LifeSpanJitter	7
	End

	Event
		Type	Start
		At	WepLOffset	
		Part	:HeavyEnergyBlastCoreWhite.part
		Magnet	Target
		Lookat	Target
		LifeSpan	26
		LifeSpanJitter	7
	End

	Event
		Type	Start
		At	WepLOffset	
		Part	:HeavyEnergyBlast.part
		Part	:HeavyEnergyBlastNavy.part
		Magnet	Target
		Lookat	Target
		LifeSpan	5

	End

	Event
		Type	Start
		At	WepLOffset	
		Part	:NavyStreaks.part
		Part	:whiteStreaks.part
		Magnet	Target
		Lookat	Target
		LifeSpan	10
	End
End


Condition
	On 	Time
	Time 	78

	Event
		ENAME   prime1
		Type	start 
		At	WepLOffset
		Bhvr	:Projectile.bhvr
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
			#PositionOffset	0 -0.25 0
		End
		Magnet	Target
	End

	Event
		Ename hook
		Type	local
		At	prime1
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1		
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
	
	Event
		Type	local
		At	prime1
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1		
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
End

Condition
	On 	Time
	Time 	82

	Event
		ENAME   prime2
		Type	start 
		At	WepLOffset
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
			#PositionOffset	0 -0.25 0
		End
		Bhvr	:Projectile.bhvr
		Magnet	Target
	End

	Event
		Ename hook
		Type	local
		At	prime2
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
	
	Event
		Type	local
		At	prime2
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
End

Condition
	On 	Time
	Time 	85

	Event
		ENAME   prime3
		Type	start 
		At	WepLOffset
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1
			#PositionOffset	0 -0.25 0
		End
		Bhvr	:Projectile.bhvr
		Magnet	Target
	End

	Event
		Ename hook
		Type	local
		At	prime3

		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1		
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
	
	Event
		Type	local
		At	prime3
		Bhvroverride
			StartJitter	1 1 1
			Spin		4 1 1	
		End
		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
	End
End

## CLEAN UP ###########################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

Condition
	On	Prime1Hit

	Event
		Ename	Prime1
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		Ename	Prime3
		Type	Destroy
	End
End

#############################################################

End				