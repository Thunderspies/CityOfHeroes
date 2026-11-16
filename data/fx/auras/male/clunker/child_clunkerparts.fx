#############################################################
## Child_ClunkerParts.fx
#############################################################

FxInfo
#############################################################

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:BoltBurst_Long.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:BoltBurst_Long.part
		Lifespan 	6
		LifespanJitter	4
	End
End

###

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:BoltBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:BoltBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End

###

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:ScrewBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:ScrewBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End

###

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:ScrewBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:ScrewBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End

#####

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:WasherBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:WasherBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End

########

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:WasherBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Time
	Time	0
	Chance	0.1
	Event
		Type	Local
		At	Origin
		Part	:WasherBurst_Short.part
		Lifespan 	6
		LifespanJitter	4
	End
End


##########################

Condition
	On	Time
	Time	0
	Chance	0.3
	Event
		Type PositOnly
		At Origin
		Bhvr	Behaviors/Braz/GeoBurst.bhvr
		BhvrOverride
			Scale			.04 .04 .04
			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.1 0.15 0.1
			SpinJitter		0.2 .1 .2
			Gravity			0.02
			TintGeom		1
		End
		Geom	Steampunk_Spring
	End
End

Condition
	On	Time
	Time	0
	Chance	0.3
	Event
		Type PositOnly
		At Origin
		Bhvr	Behaviors/Braz/GeoBurst.bhvr
		BhvrOverride
			Scale			.03 .03 .03
			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.1 0.15 0.1
			SpinJitter		0.2 .1 .2
			Gravity			0.02
			TintGeom		1
		End
		Geom	Steampunk_Spring
	End
End

#############

Condition
	On	Time
	Time	0
	Chance	0.2
	Event
		Type PositOnly
		At Origin
		Bhvr	Behaviors/Braz/GeoBurst.bhvr
		BhvrOverride
			Scale			.04 .04 .04
			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.1 0.15 0.1
			SpinJitter		0.2 .1 .2
			Gravity			0.02
			TintGeom		1
		End
		Geom	Steampunk_Gear01
	End
End

Condition
	On	Time
	Time	0
	Chance	0.2
	Event
		Type PositOnly
		At Origin
		Bhvr	Behaviors/Braz/GeoBurst.bhvr
		BhvrOverride
			Scale			.03 .03 .03
			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.1 0.15 0.1
			SpinJitter		0.2 .1 .2
			Gravity			0.02
			TintGeom		1
		End
		Geom	Steampunk_Gear01
	End
End

#############

Condition
	On	Time
	Time	0
	Chance	0.2
	Event
		Type PositOnly
		At Origin
		Bhvr	Behaviors/Braz/GeoBurst.bhvr
		BhvrOverride
			Scale			.04 .04 .04
			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.1 0.15 0.1
			SpinJitter		0.2 .1 .2
			Gravity			0.02
			TintGeom		1
		End
		Geom	Steampunk_Gear02
	End
End

Condition
	On	Time
	Time	0
	Chance	0.2
	Event
		Type PositOnly
		At Origin
		Bhvr	Behaviors/Braz/GeoBurst.bhvr
		BhvrOverride
			Scale			.03 .03 .03
			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.1 0.15 0.1
			SpinJitter		0.2 .1 .2
			Gravity			0.02
			TintGeom		1
		End
		Geom	Steampunk_Gear02
	End
End

#############################################################
End