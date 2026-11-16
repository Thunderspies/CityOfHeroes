#########################################################
##	RadiationAura
FxInfo

LifeSpan	160

###########################################################################################
##

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound transformrikti 80 80 .95
	End
End






Condition

	Event
		Ename	Node1
		Type	start
		At	origin
		
		ChildFx	EnemyFx/Rikti/WhiteNoise.fx
		
		Lifespan	80
	End

End

Condition

	Event
		Ename	Node1
		Type	local
		At	Root
		
		ChildFx	:RiktiArachnosDescramblerLines.fx
		
		Lifespan	110
	End

End

Condition
	On	Time
	Time	5

	Event
		Ename	Node1
		Type	local
		At	Root
		
		ChildFx	:RiktiArachnosDescramblerLines.fx
		
		Lifespan	105
	End

End

Condition
	On	Time
	Time	10

	Event
		Ename	Node1
		Type	local
		At	Root
		
		ChildFx	:RiktiArachnosDescramblerLines.fx
		
		Lifespan	100
	End

End

Condition
	On	Time
	Time	15

	Event
		Ename	Node1
		Type	local
		At	Root
		
		ChildFx	:RiktiArachnosDescramblerLines.fx
	
		Lifespan	95
	End

End

Condition
	On	Time
	Time	20

	Event
		Ename	Node1
		Type	local
		At	Root
		
		ChildFx	:RiktiArachnosDescramblerLines.fx
	
		Lifespan	90
	End

End

#############################################################

Condition

	Event
		Type	local
		At	UarmL
		

		ChildFx	POWERS\ElectricityControl\ChragedBrawlHitNoCore.fx
		LifeSpan	10
	End

	Event	
		Type	local
		At	origin
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End

Condition
	On	Time
	Time	3

	Event	
		Type	local
		At	origin
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End


Condition
	On	Time
	Time	8

	Event	
		Type	local
		At	LlegR
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End

Condition
	On	Time
	Time	12

	Event	
		Type	local
		At	Larmr
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End

Condition
	On	Time
	Time	18

	Event	
		Type	local
		At	LlegL
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End

##########################################################################################################

Condition
	On	Time
	Time	23

	Event	
		Type	local
		At	LlegL
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End


Condition
	On	Time
	Time	38

	Event	
		Type	local
		At	LlegL
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End


Condition
	On	Time
	Time	42

	Event	
		Type	local
		At	LlegL
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End


Condition
	On	Time
	Time	48

	Event	
		Type	local
		At	LlegL
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End


Condition
	On	Time
	Time	53

	Event	
		Type	local
		At	LlegL
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End


Condition
	On	Time
	Time	58

	Event	
		Type	local
		At	LlegL
		
		ChildFx	:RiktiArachnosDescramblerElectricitySparks.fx
		Lifespan	95
	End

End
##########################################################################################


###################################################################################
##########################################################

Condition

	Event
		Type	start
		At	chest

		Bhvr	V_COV/Powers/electricalMelee/HazeFadeOut.bhvr
		Part	:ElectricityBlastHaze.part

		LifeSpan	130
	End

	Event
		EName	Ring2
		Type	start
		At	chest
		
		Part	:SpeckHaze.part
		Part	:SpeckHazeRed.part
		Part	:SpeckHazeBlue.part

		Part	:RegenBodyGlowsDark.part
		Part	:Ripples.part	

		Part	:RippleRings.part
		Part	:RippleThinRings.part
		LifeSpan	110
	End

	Event
		Type	start
		At	chest
		
		BhvrOverride
		
			Hueshift	180
			HueshiftJitter	180
		
		End

		Part	:RippleColor.part	
		
		LifeSpan	110
	End
	
	Event
		Type	start
		At	chest
		
		BhvrOverride
		
			Hueshift	180
			HueshiftJitter	180
		
		End

		Part	:RippleColor.part	
		
		LifeSpan	110
	End
	
	Event
		Type	start
		At	chest
		
		BhvrOverride
		
			Hueshift	180
			HueshiftJitter	180
		
		End

		Part	:RippleColor.part	
		
		LifeSpan	110
	End
	
	Event
		Type	start
		At	chest
		
		BhvrOverride
		
			Hueshift	180
			HueshiftJitter	180
		
		End

		Part	:RippleColor.part	
		
		LifeSpan	110
	End

End

End				