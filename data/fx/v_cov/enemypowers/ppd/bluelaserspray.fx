#########################################################
##	
##

FxInfo

LifeSpan  150

##################################

Condition
	On 	Time
	Time 	35


	Event
		ENAME   P2
		Type	Local 
		At	LarmR	
		BhvrOverride
			PositionOffset	2 -.05 -.35
		End
		
	End

	Event
		Type	Start
		At	p2
		Part	:LaserBlastOriginStar.part
		Part	:LaserBlastOriginEnergy.part
		Part	:LaserBlastOriginEnergyBlue.part
		Sound RobotLaserRifleBurst3b 90 90 .97
		LifeSpan	10
	End

	Event
		EName 	ConeTarg
		Type	Start
		At	Target
		Lookat	p2

	End
	
	Event
		EName 	Aim
		Type	Local
		At	ConeTarg
		BhvrOverride
			PositionOffset	0 0 -40
			StartJitter	12 3 5
		End
		Lookat	p2

	End
	
	Event
		EName 	Prime
		Type	Start
		At	p2
		
		Part	:LaserPulseFlash.part
		Part	:LaserPulseCore.part
		Part	:LaserPulseGlow.part
		Part	:LaserPulseStreak.part

		Lookat	Aim
		Magnet	Aim
		BHVR	:LaserProjectile.bhvr

		Lifespan	30

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
		#Sound	RobotLaserRifleBurst3 90 70 .8
		LifeSpan	10
	End

	Event
		EName 	Aim1
		Type	Local
		At	ConeTarg
		BhvrOverride
			PositionOffset	0 0 -35
			StartJitter	11 3 5
		End
		Lookat	p2


	End
	
	Event
		EName 	Prime1
		Type	Start
		At	p2
		
		Part	:LaserPulseFlash.part
		Part	:LaserPulseCore.part
		Part	:LaserPulseGlow.part
		Part	:LaserPulseStreak.part

		Lookat	Aim1
		Magnet	Aim1
		BHVR	:LaserProjectile.bhvr

		Lifespan	30

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
		#Sound	RobotLaserRifleBurst3 90 70 .8
		LifeSpan	10
	End

	Event
		EName 	Aim2
		Type	Local
		At	ConeTarg
		BhvrOverride
			PositionOffset	0 0 -30
			StartJitter	10 3 5
		End
		Lookat	p2


	End
	
	Event
		EName 	Prime2
		Type	Start
		At	p2
		
		Part	:LaserPulseFlash.part
		Part	:LaserPulseCore.part
		Part	:LaserPulseGlow.part
		Part	:LaserPulseStreak.part

		Lookat	Aim2
		Magnet	Aim2
		BHVR	:LaserProjectile.bhvr

		Lifespan	30

	End

	
End

Condition
	On 	Time
	Time 	50


	Event
		Type	Start
		At	p2
		Part	:LaserBlastOriginStar.part
		Part	:LaserBlastOriginEnergy.part
		Part	:LaserBlastOriginEnergyBlue.part
		#Sound	RobotLaserRifleBurst3 90 70 .8
		LifeSpan	10
	End

	Event
		EName 	Aim3
		Type	Local
		At	ConeTarg
		BhvrOverride
			PositionOffset	0 0 -25
			StartJitter	9 3 5
		End
		Lookat	p2


	End
	
	Event
		EName 	Prime3
		Type	Start
		At	p2
		
		Part	:LaserPulseFlash.part
		Part	:LaserPulseCore.part
		Part	:LaserPulseGlow.part
		Part	:LaserPulseStreak.part

		Lookat	Aim3
		Magnet	Aim3
		BHVR	:LaserProjectile.bhvr

		Lifespan	30

	End

End


#Condition
#	On 	Time
#	Time 	55
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim4
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -20
#			StartJitter	8 3 5
#		End
#		Lookat	p2
#
#
#	End
#	
#	Event
#		EName 	Prime4
#		Type	Start
#		At	p2
#		
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#		Lookat	Aim4
#		Magnet	Aim4
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#End

#Condition
#	On 	Time
#	Time 	60
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim5
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -15
#		End
#		Lookat	p2
#
#	End
#	
#	Event
#		EName 	Prime5
#		Type	Start
#		At	p2
#	
#
#		Lookat	Aim5
#		Magnet	Aim5
#
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#	
#	
#End

#Condition
#	On 	Time
#	Time 	65
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim6
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -40
#			StartJitter	12 3 5
#		End
#		Lookat	p2
#
#	End
#	
#	Event
#		EName 	Prime6
#		Type	Start
#		At	p2
#		
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#		Lookat	Aim6
#		Magnet	Aim6
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	70
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim7
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -35
#			StartJitter	11 3 5
#		End
#		Lookat	p2
#
#
#	End
#	
#	Event
#		EName 	Prime7
#		Type	Start
#		At	p2
#		
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#		Lookat	Aim7
#		Magnet	Aim7
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#
#	
#End
#
#Condition
#	On 	Time
#	Time 	75
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim8
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -30
#			StartJitter	10 3 5
#		End
#		Lookat	p2
#
#
#	End
#	
#	Event
#		EName 	Prime8
#		Type	Start
#		At	p2
#		
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#		Lookat	Aim8
#		Magnet	Aim8
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#
#	
#End
#
#Condition
#	On 	Time
#	Time 	80
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim9
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -25
#			StartJitter	9 3 5
#		End
#		Lookat	p2
#
#
#	End
#	
#	Event
#		EName 	Prime9
#		Type	Start
#		At	p2
#		
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#		Lookat	Aim9
#		Magnet	Aim9
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	85
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim10
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -20
#			StartJitter	8 3 5
#		End
#		Lookat	p2
#
#
#	End
#	
#	Event
#		EName 	Prime10
#		Type	Start
#		At	p2
#		
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#		Lookat	Aim10
#		Magnet	Aim10
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#End
#
#Condition
#	On 	Time
#	Time 	90
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim1
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -15
#		End
#		Lookat	p2
#
#	End
#	
#	Event
#		EName 	Prime1
#		Type	Start
#		At	p2
#	
#
#		Lookat	Aim1
#		Magnet	Aim1
#
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#	
#	
#End
#
#Condition
#	On 	Time
#	Time 	95
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim2
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -30
#			StartJitter	10 3 5
#		End
#		Lookat	p2
#
#
#	End
#	
#	Event
#		EName 	Prime2
#		Type	Start
#		At	p2
#		
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#		Lookat	Aim2
#		Magnet	Aim2
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#
#	
#End
#
#Condition
#	On 	Time
#	Time 	100
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim3
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -25
#			StartJitter	9 3 5
#		End
#		Lookat	p2
#
#
#	End
#	
#	Event
#		EName 	Prime3
#		Type	Start
#		At	p2
#		
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#		Lookat	Aim3
#		Magnet	Aim3
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	105
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim4
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -20
#			StartJitter	8 3 5
#		End
#		Lookat	p2
#
#
#	End
#	
#	Event
#		EName 	Prime4
#		Type	Start
#		At	p2
#		
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#		Lookat	Aim4
#		Magnet	Aim4
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#End
#
#Condition
#	On 	Time
#	Time 	110
#
#
#	Event
#		Type	Start
#		At	p2
#		Part	:LaserBlastOriginStar.part
#		Part	:LaserBlastOriginEnergy.part
#		Part	:LaserBlastOriginEnergyBlue.part
#		Sound	RobotLaserRifleBurst3 90 70 .8
#		LifeSpan	10
#	End
#
#	Event
#		EName 	Aim5
#		Type	Local
#		At	ConeTarg
#		BhvrOverride
#			PositionOffset	0 0 -15
#		End
#		Lookat	p2
#
#	End
#	
#	Event
#		EName 	Prime5
#		Type	Start
#		At	p2
#	
#
#		Lookat	Aim5
#		Magnet	Aim5
#
#		Part	:LaserPulseFlash.part
#		Part	:LaserPulseCore.part
#		Part	:LaserPulseGlow.part
#		Part	:LaserPulseStreak.part
#
#
#		BHVR	:LaserProjectile.bhvr
#
#		Lifespan	30
#
#	End
#	
#	
#End

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

Condition
	On	Prime4Hit
	
	Event
		Ename	Prime4
		Type	Destroy
	End

End

#Condition
#	On	Prime5Hit
#	
#	Event
#		Ename	Prime5
#		Type	Destroy
#	End
#
#End

#Condition
#	On	Prime6Hit
#	
#	Event
#		Ename	Prime6
#		Type	Destroy
#	End
#
#End

Condition
	On	Prime7Hit
	
	Event
		Ename	Prime7
		Type	Destroy
	End

End

Condition
	On	Prime8Hit
	
	Event
		Ename	Prime8
		Type	Destroy
	End

End

Condition
	On	Prime9Hit
	
	Event
		Ename	Prime9
		Type	Destroy
	End

End

Condition
	On	Prime10Hit
	
	Event
		Ename	Prime10
		Type	Destroy
	End

End

End				