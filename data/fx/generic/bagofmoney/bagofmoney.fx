#####################
## Bag of Money FX ##
#####################

FxInfo

LifeSpan	0

##############
## Constant ##
##############

Condition
	On 	Time  

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	Back					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	GEO_MoneyBag_Black			#call to geometry fx
		Geom	GEO_MoneyBag_Olive			#call to geometry fx
		Geom	GEO_MoneyBag_Canvas			#call to geometry fx
	End

End

Condition
	On		Cycle
	Time		30
	#DoMany		1
	Chance		0.125
	

	Event   
		Type	start
		At	back

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		0.0 0.125 0.0
			InitialVelocityJitter	0.125 0.125 0.125
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End

		CThresh 0.0001
		Lifespan	300
		LifespanJitter	60
	End

End


#########################
## Trigger Bits - DEATH ##
#########################


Condition
	On		TriggerBits
	TriggerBits	DEATH
	#DoMany		1

	
	Event
		Type		start
		At		back
		ChildFX		:BagOfMoney_DeathExplosion.fx
	End

End

#############################
## Trigger bit - KNOCKBACK ##
#############################

Condition
	On		TriggerBits
	TriggerBits	KNOCKBACK
	DoMany		0
	Chance		0.25
	
	Repeat		11
	RepeatJitter	4

	Event   
		Type	start
		At	back

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.25 0.5 .25
			InitialVelocityJitter	0.5 0.5 0.5
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End

		CThresh 0.0001
		Lifespan	300
		LifespanJitter	60
	End

End


#######################
## Trigger bit - AIR ##
#######################

Condition
	On		TriggerBits
	TriggerBits	AIR
	DoMany		0
	Chance		0.25
	
	Repeat		1
	RepeatJitter	1

	Event   
		Type	start
		At	hips

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .1
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.5 0.5
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End

		CThresh 0.0001
		Lifespan	300
		LifespanJitter	60
	End

End

###########################
## Trigger bit - FORWARD ##
###########################

Condition
	On		TriggerBits
	TriggerBits	FORWARD
	DoMany		1
	Chance		0.0625

	Repeat		0
	RepeatJitter	2

	Event   
		Type	start
		At	back

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		0.2 0.1 0.1
			InitialVelocity		0.125 0.125 0.125
			InitialVelocityJitter	0.125 0.25 0.125
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		0.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End

		CThresh 0.0001
		Lifespan	300
		LifespanJitter	60
	End

End

############################
## Trigger bit - BACKWARD ##
############################

Condition
	On		TriggerBits
	TriggerBits	BACKWARD
	DoMany		1
	Chance		0.0625

	Repeat		0
	RepeatJitter	2

	Event   
		Type	start
		At	chest

		Geom	GreenBacks
		Geom	GreenBacks02
		Geom	GreenBacks03

		#Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		0.2 0.1 0.1
			InitialVelocity		0.125 0.125 0.125
			InitialVelocityJitter	0.125 0.25 0.125
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		0.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysScale		1.5 2 1.5
			PhysDebris		1
			PhysDensity		3
		End

		CThresh 0.0001
		Lifespan	300
		LifespanJitter	60
	End

End


End