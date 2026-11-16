#########################################################
##	template

FxInfo


Flags    InheritAlpha

LifeSpan	220

#########################################################
####################### AUDIO ###########################
#########################################################

#Condition
#
#	On 	Time	
#	Time	18
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans4 80 30 .88
#	End
#
#End
#
#Condition
#
#	On 	Time	
#	Time	70
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans3 80 30 .78
#	End
#
#End


#########################################################
################## Right Side Ports #######################
#########################################################

Condition

	Event
		Ename	vent01
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		58 -10 26
		End

		LifeSpan	25
		
	End

End

Condition
	On	Cycle
	Time	1

	Event
		Ename	Prime3
		Type	start
		At	vent01
		bhvr	:projectile1.bhvr
		
		Part	:EnergyStreak.part
		Part	:EnergyGasses.part
		Part	:EnergyGasses2.part
		
		Magnet	TargetNodeRight

		LifeSpan	22

	End
	
End


#########################################################
Condition

	Event
		Ename	vent02
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		38 -8 26
		End

		LifeSpan	22
		
	End
	
End

Condition
	On	Cycle
	Time	1

	Event
		Ename	Prime4
		Type	start
		At	vent02
		bhvr	:projectile1.bhvr
		
		Part	:EnergyStreak.part
		Part	:EnergyGasses.part
		Part	:EnergyGasses2.part
		
		Magnet	TargetNodeRight

		LifeSpan	22

	End
	
End
#########################################################
######################################
Condition

	Event
		Ename	vent03
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		17 -7 26
		End

		LifeSpan	25
		
	End
	
End

Condition
	On	Cycle
	Time	1

	Event
		Ename	Prime5
		Type	start
		At	vent03
		bhvr	:projectile1.bhvr
		
		Part	:EnergyStreak.part
		Part	:EnergyGasses.part
		Part	:EnergyGasses2.part
		
		Magnet	TargetNodeRight

		LifeSpan	22

	End
	
End
#########################################################
################## Left Side Ports #######################
#########################################################
Condition

	Event
		Ename	Lvent01
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		58 -10 -26
		End

		LifeSpan	25
		
	End

End

Condition
	On	Cycle
	Time	1

	Event
		Ename	Prime
		Type	start
		At	Lvent01
		bhvr	:projectile1.bhvr
		
		Part	:EnergyStreak.part
		Part	:EnergyGasses.part
		Part	:EnergyGasses2.part
		
		Magnet	TargetNodeLeft

		LifeSpan	22

	End
	
End

###############################################
##Power Projectiles sent towards target
#########################################################

Condition
	On	Time
	Time	18
	
	Event
		Ename	Prime6
		Type	start
		At	TargetNodeRight
		bhvr	:projectile.bhvr
		
		Part	:ProjectileEnergyStreak.part
		Part	:ProjectileEnergyGasses.part
		Part	:ProjectileEnergyGasses2.part
		Part	:ProjectileEnergyGasses.part
		Part	:ProjectileEnergyGasses2.part
		Part	:ProjectileEnergyGasses.part
		Part	:ProjectileEnergyGasses2.part
		Part	:ProjectileEnergyGasses.part
		Part	:ProjectileEnergyGasses2.part
		
		Magnet	Target
		LifeSpan	252

	End
	
	Event
		Ename	Prime7
		Type	start
		At	TargetNodeLeft
		bhvr	:projectile.bhvr
		
		Part	:ProjectileEnergyStreak.part
		Part	:ProjectileEnergyGasses.part
		Part	:ProjectileEnergyGasses2.part
		Part	:ProjectileEnergyGasses.part
		Part	:ProjectileEnergyGasses2.part
		Part	:ProjectileEnergyGasses.part
		Part	:ProjectileEnergyGasses2.part
		Part	:ProjectileEnergyGasses.part
		Part	:ProjectileEnergyGasses2.part
		
		Magnet	Target
		LifeSpan	252

	End

End

##########################################################

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

Condition
	On	Prime5Hit

	Event
		Ename	Prime5
		Type	Destroy
	End
	
End

Condition
	On	Prime6Hit

	Event
		Ename	Prime6
		Type	Destroy
	End

End

Condition
	On	Prime7Hit

	Event
		Ename	Prime7
		Type	Destroy
	End

End
#########################################################

Condition

	Event
		Ename	Lvent02
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		38 -9 -26
		End

		LifeSpan	22
		
	End
End

Condition
	On	Cycle
	Time	1

	Event
		Ename	Prime1
		Type	start
		At	Lvent02
		bhvr	:projectile1.bhvr
		
		Part	:EnergyStreak.part
		Part	:EnergyGasses.part
		Part	:EnergyGasses2.part
		
		Magnet	TargetNodeLeft

		LifeSpan	22

	End
End

##########################################################

Condition

	Event
		Ename	Lvent03
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		17 -7 -26
		End

		LifeSpan	25

	End

End

Condition
	On	Cycle
	Time	1

	Event
		Ename	Prime2
		Type	start
		At	Lvent03
		bhvr	:projectile1.bhvr
		
		Part	:EnergyStreak.part
		Part	:EnergyGasses.part
		Part	:EnergyGasses2.part
		
		Magnet	TargetNodeLeft

		LifeSpan	22

	End
	
End
##########################################################
#######################Offset Points Left ##################
##########################################################
Condition

	Event
		Ename	TargetNodeRight
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		17 -60 126
		End
		
		Part	:BlinkingLights.part
		LifeSpan	25
	End

End

##########################################################
#######################Offset Points Right ##################
##########################################################
Condition

	Event
		Ename	TargetNodeLeft
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		17 -60 -126
		End
		
		Part	:BlinkingLights.part
		LifeSpan	25
	End

End

End		