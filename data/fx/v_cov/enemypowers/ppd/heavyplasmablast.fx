#########################################################
##	
##

FxInfo

LifeSpan  400

##################################

#76		+40

##################################

Condition
	On	Time
	Time	13
	

	Event
		Type	Local
		At 	C_Head
		Sound HeavyPlasmaBlast2 100 100 .9
	End
End

Condition
	On	Time
	Time	26
			
	Event
		Type	local
		At	C_Head
		
#streaklines Initial
		BhvrOverride
			PositionOffset	0 -0.2 -2.2
		End

		Part	:EnergyLiquidFirst.part

		Magnet	Target
		Lookat	Target
		LifeSpan	47

	End

End

Condition
	On	Time
	Time	26
	
	Event
		Type	local
		At	C_Head
		
#streaklines Initial
		BhvrOverride
			PositionOffset	0 -0.2 -2.2
		End

		Part	:EnergyLiquid.part

		Magnet	Target
		Lookat	Target
		LifeSpan	47

	End

End

Condition
	On	Time
	Time	26
	
	Event
		Type	local
		At	C_Head
		
#streaklines Initial

		BhvrOverride
			PositionOffset	0 -0.2 -2.2
		End

		Part	:mainEnergyCore.part
		Part	:MainEnergyRing.part
		Part	:MainEnergyThinRing.part
		Part	:NavyStreaksIn.part
		Part	:WhiteStreaksIn.part
		Magnet	Target
		Lookat	Target
		LifeSpan	47

	End
End

Condition
	On 	Time
	Time 	74

	Event
		Type	Local
		At	C_Head
		
		BhvrOverride
			PositionOffset	0 0 -2
		End

		Part	:Blast3.part
		Part	:Blast2.part
		LifeSpan	4

	End

	Event
		Type	Local
		At	C_Head
		
		BhvrOverride
			PositionOffset	0 0 -2
		End

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
		At	c_Head


		Bhvroverride
		
			StartJitter	1 1 1
			Spin		4 1 1	
			PositionOffset	0 -.25 -2.2
		End
		

		Bhvr	:Projectile.bhvr
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
#		Part4	:CometHead.part
		Part5	:EnergyCoreLocal.part
	End
End

Condition
	On 	Time
	Time 	78

	Event
		ENAME   prime1
		Type	start 
		At	c_Head

		Bhvroverride
		
			StartJitter	1 1 1
			Spin		4 1 1	
			PositionOffset	0 -.25 -2.2
		End
		Bhvr	:Projectile.bhvr
		Magnet	Target
	End

	Event
		Ename hook
		Type	local
		At	prime1

		Bhvroverride
		
			StartJitter	1 1 1
			Spin		4 1 1		
			PositionOffset	0 0 3
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
			PositionOffset	0 0 3
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
		At	c_Head

		Bhvroverride
		
			StartJitter	1 1 1
			Spin		4 1 1	
			PositionOffset	0 -.25 -2.2
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
			PositionOffset	0 0 3
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
			PositionOffset	0 0 3
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
		At	c_Head

		Bhvroverride
		
			StartJitter	1 1 1
			Spin		4 1 1
			PositionOffset	0 -.25 -2.2
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
			PositionOffset	0 0 3
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
			PositionOffset	0 0 3
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
		At	C_Head	

		Bhvroverride
			PositionOffset	0 -.25 -2.2
		End

		Part	:PlasmaTrail.part

		Magnet	Target
		Lookat	Target

		LifeSpan	20
	End

	Event
		
		Ename	Blasthead
		Type	Start
		At	C_Head	

		Bhvroverride
			PositionOffset	0 -.25 -2.2
		End		

		Part	:HeavyVillianSniper_Tracer.part
		Part	:HeavyVillianSniper_Tracer.part
		Part	:HeavyVillianSniper_Tracer.part

		Part	:HeavyVillianSniper_Head.part
		
		#BHVR	Behaviors/GenericParticleFade.bhvr

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
			PositionOffset	0 0 3
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
			PositionOffset	0 0 3
		End

		Part	:EnergyCore.part
		Part	:EnergyDroplet.part
		

	End

	Event
		Type	Start
		At	C_Head	

		Bhvroverride
			PositionOffset	0 -.25 -2.2
		End	
		
		Part	:HeavyEnergyBlast3.part
		Part	:HeavyEnergyBlast2.part
		Part	:HeavyEnergyBlast3.part
		Part	:HeavyEnergyBlast2.part


		Magnet	Target
		Lookat	Target
		LifeSpan	20
	End
	
	Event
		Type	Start
		At	C_Head	

		Bhvroverride
			PositionOffset	0 -.25 -2.2
		End	
		
		Part	:HeavyEnergyBlastHead2.part


		Magnet	Target
		Lookat	Target
		LifeSpan	18
	End

	Event
		Type	Start
		At	C_Head	

		Bhvroverride
			PositionOffset	0 -.25 -2.2
		End	
		
		Part	:HeavyEnergyBlastCore.part
		Part	:HeavyEnergyBlastCoreInvert.part

		Magnet	Target
		Lookat	Target
		LifeSpan	20
		LifeSpanJitter	7
	End

	Event
		Type	Start
		At	C_Head	

		Bhvroverride
			PositionOffset	0 -.25 -2.2
		End	
		
		Part	:HeavyEnergyBlastCoreWhite.part

		Magnet	Target
		Lookat	Target
		LifeSpan	26
		LifeSpanJitter	7
	End

	Event
		Type	Start
		At	C_Head	

		Bhvroverride
			PositionOffset	0 -.25 -2.2
		End	
		
#streaklines Initial

		Part	:HeavyEnergyBlast.part
		Part	:HeavyEnergyBlastNavy.part

		Magnet	Target
		Lookat	Target
		LifeSpan	5

	End


	Event
		Type	Start
		At	C_Head	

		Bhvroverride
			PositionOffset	0 -.25 -2.2
		End	
		
#streaklines

		Part	:NavyStreaks.part
		Part	:whiteStreaks.part

		Magnet	Target
		Lookat	Target
		LifeSpan	10
	End
End

##################################


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

End				