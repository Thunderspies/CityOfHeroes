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

		LifeSpan	9	#25
		
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

		LifeSpan	5	#22
		
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

		LifeSpan	9	#25
		
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

		LifeSpan	9	#25
		
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

##########################################################
#######################Offset Points Left ##################
##########################################################
Condition

	Event
		Ename	TargetNodeRight
		Type	local
		At	root

		BhvrOverride
			PositionOffset		17 -60 126
		End
		
		ChildFx	:LargeRiktiPortal.fx

		LifeSpan	95
	End

End

Condition
		
	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	13

	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	15

	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

	Event

		Type	local
		At	root

		BhvrOverride
			PositionOffset		17 -60 126
		End

		Part	:LargeEnergyRings.part
		Part	:LargeHitEnergyGasses.part
		Part	:LargeHitEnergyGasses2.part

		LifeSpan	20
	End

End


Condition
	On	Time
	Time	18

	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End



Condition
	On	Time
	Time	19

	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	21

	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	26

	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	29

	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	30

	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	45

	Event
		Type	local
		At	TargetNodeRight

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

##########################################################
#######################Offset Points Left ##################
##########################################################
Condition

	Event
		Ename	TargetNodeLeft
		Type	local
		At	root

		BhvrOverride
			PositionOffset		17 -60 -126
		End
		
		ChildFx	:LargeRiktiPortal.fx

		LifeSpan	95
	End


End

Condition
	On	Time
	Time	13

	Event
		Type	local
		At	TargetNodeLeft

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	15

	Event
		Type	local
		At	TargetNodeLeft

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

	Event

		Type	local
		At	root

		BhvrOverride
			PositionOffset		17 -60 -126
		End

		Part	:LargeEnergyRings.part		
		Part	:LargeHitEnergyGasses.part
		Part	:LargeHitEnergyGasses2.part

		LifeSpan	20
	End
End


Condition
	On	Time
	Time	18

	Event
		Type	local
		At	TargetNodeLeft

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End



Condition
	On	Time
	Time	19

	Event
		Type	local
		At	TargetNodeLeft

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	21

	Event
		Type	local
		At	TargetNodeLeft

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	26

	Event
		Type	local
		At	TargetNodeLeft

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	29

	Event
		Type	local
		At	TargetNodeLeft

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	30

	Event
		Type	local
		At	TargetNodeLeft

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
	End

End

Condition
	On	Time
	Time	45

	Event
		Type	local
		At	TargetNodeLeft

		BhvrOverride
			StartJitter		40.0 40.0 40.0
		End
		
		ChildFx	:RiktiPortal3.fx

		LifeSpan	45
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

#########################################################

Condition

	Event
		Ename	Lvent02
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		38 -9 -26
		End

		LifeSpan	5	#22
		
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

		LifeSpan	9	#25

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

End		