#########################################################
## Staff of the M.A.G.I. - Ball Lightning
#########################################################

FxInfo

LifeSpan	150

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound Mu_elecball 80 80 .93
	End
End

## CAST FX #######################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	StaffEnd
		Type	Local 
		At	WepR
		BhvrOverride
			PositionOffset	0 0 2
		End
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	local
		At	StaffEnd
		Part1	:BallLightning_Control01.part
		Part2	:BallLightning_Control02.part
		Part3	:BallLightning_FlareGlow.part
		Part3	:BallLightning_Ring1.part
		LifeSpan	20
	End
End

Condition
	On 	Time
	Time 	25
	
	Event
		Type	local
		At	StaffEnd
		Part4	:BallLightning_SparkBurst1.part
		LifeSpan	20
	End
End

##########################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime
		Type	start
		At	StaffEnd
		Bhvr	Behaviors/Medianprojectile1.bhvr
		Part1	:BallLightning_Control01.part
		Part2	:BallLightning_Control02.part
		Part3	:BallLightning_FlareGlow.part
		Part3	:BallLightning_FlareCore.part
		Magnet	Target
		Lookat	Target
	End
End

#########################################################

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		Type	Local
		At	Target
		Sound elecballhit 60 60 .6
		Part1	:BallLightning_Arcs1.part
		Part2	:BallLightning_Arcs2.part
		Part3	:BallLightning_Arcs3.part
		lifespan	25
	End

##	Event
##		
##		Type	Local
##		At	Target
##		Part2	:BallLightning_Glow.part
##		lifespan	5
##	End

	Event
		Type	Start
		At	Target
		Part1	:BallLightning_SparkBurst1.part
		Part2	:BallLightning_SparkBurst1.part
		lifespan	30
	End
End

#########################################################

End