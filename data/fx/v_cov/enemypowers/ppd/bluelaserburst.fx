#########################################################
##	
##

FxInfo

LifeSpan  400

##################################

Condition
	On 	Time
	Time 	30

	Event
		ENAME   p2
		Type	Local 
		At	LarmL
		BhvrOverride
			PositionOffset	-2 -.3 -0.35
		End
	End

	Event
		Type	Start
		At	p2
		Part	:LaserBlastOriginStar.part
		Part	:LaserBlastOriginEnergy.part
		Part	:LaserBlastOriginEnergyBlue.part
		Sound BLueLaser1 90 90 .8
		LifeSpan	10
	End

	Event
		EName 	Prime
		Type	Start
		At	p2
		
		Part	:LaserPulseFlash.part
		Part	:LaserPulseCore.part
		Part	:LaserPulseGlow.part
		Part	:LaserPulseStreak.part


		BHVR	:LaserProjectile.bhvr

		Magnet	target
		Lookat	target

	End
	
End

Condition
	On 	Time
	Time 	35


	Event
		Type	Start
		At	p2
		Part	:LaserBlastOriginStar.part
		Part	:LaserBlastOriginEnergy.part
		Part	:LaserBlastOriginEnergyBlue.part
		Sound BLueLaser1 90 90 .8
		LifeSpan	10
	End

	Event
		EName 	Prime1
		Type	Start
		At	p2
		
		Part	:LaserPulseFlash.part
		Part	:LaserPulseCore.part
		Part	:LaserPulseGlow.part
		Part	:LaserPulseStreak.part


		BHVR	:LaserProjectile.bhvr

		Magnet	target
		Lookat	target

	End
	
End

Condition
	On 	Time
	Time 	40


	Event
		Type	Start
		At	p2
		Part	:LaserBlastOriginStar.part
		Part	:LaserBlastOriginEnergy.part
		Part	:LaserBlastOriginEnergyBlue.part
		Sound BLueLaser1 90 90 .8
		LifeSpan	10
	End

	Event
		EName 	Prime2
		Type	Start
		At	p2
		
		Part	:LaserPulseFlash.part
		Part	:LaserPulseCore.part
		Part	:LaserPulseGlow.part
		Part	:LaserPulseStreak.part


		BHVR	:LaserProjectile.bhvr

		Magnet	target
		Lookat	target

	End
	
End

Condition
	On 	Time
	Time 	45


	Event
		Type	Start
		At	p2
		Part	:LaserBlastOriginStar.part
		Part	:LaserBlastOriginEnergy.part
		Part	:LaserBlastOriginEnergyBlue.part
		Sound BLueLaser2 90 90 .8
		LifeSpan	10
	End

	Event
		EName 	Prime3
		Type	Start
		At	p2
		
		Part	:LaserPulseFlash.part
		Part	:LaserPulseCore.part
		Part	:LaserPulseGlow.part
		Part	:LaserPulseStreak.part


		BHVR	:LaserProjectile.bhvr

		Magnet	target
		Lookat	target

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