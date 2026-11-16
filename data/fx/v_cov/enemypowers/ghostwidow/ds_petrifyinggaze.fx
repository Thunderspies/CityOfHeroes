#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	root
End

Input  
	InpName	Head
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

LifeSpan	200

############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	InitialHeadMist
		Type	posit
		At	head
		
		part1	POWERS/DarknessControl/bFearMist.part
		part2	POWERS/DarknessControl/bFearMist2.part
		#Part3	POWERS/DarknessControl/bFearTenticles.part
		#Part4	POWERS/DarknessControl/bFearTenticles2.part

		
		BhvrOverride
			PositionOffset 0 .22 0
		End

		
		
		Magnet	Target
		Lookat	Target
		Sound petrify1 80 80 .8

	End

	Event
		EName 	EyeL
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset .1 .22 .35
		End
		Part	:soulFire0.part	
		Part	:soulGlow0.part
		Lifespan	25
	End

	Event
		EName 	EyeR
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset -.1 .22 .35
		End
		Part	:soulFire0.part	
		Part	:soulGlow0.part
		Lifespan	25
	End

	Event
		EName 	Center
		Type	create
		Inherit	position
		Update	position
		At	Head
				
		Lookat	target
	End

	Event
		EName 	Right
		Type	Local
		At	Center
				
		Magnet	Center

		BhvrOverride
			PositionOffset	-6 0 3
		End
	End

	Event
		EName 	Left
		Type	Local
		At	Center
				
		Magnet	Center
		
		BhvrOverride
			PositionOffset	6 0 3
		End
	End

End

Condition
	On 	Time
	Time 	30


	Event
		EName 	Rtarg
		Type	Start
		At	Right
				
		Magnet	Target

		BhvrOverride
			Trackrate	5
		End
	End

	Event
		EName 	Ltarg
		Type	Start
		At	Left
				
		Magnet	Target
		
		BhvrOverride
			Trackrate	5
		End
	End

	Event
		EName 	Prime
		Type	start 
		At	EyeR
		
		Bhvr	behaviors/slowprjct1TOROOT.bhvr
		BhvrOverride
			InitialVelocity 0 0 0
		End
		#Part5	POWERS/DarknessControl/PetrifingPuddleTrail2.Part
		part1	POWERS/DarknessControl/aFearMist.part
		part2	POWERS/DarknessControl/aFearMist2.part
		#Part3	POWERS/DarknessControl/aFearTenticles.part
		#Part4	POWERS/DarknessControl/aFearTenticles2.part

		Part	:soulSmoke0.part

		Part	:soulGlow1.part
		Part	:soulGlow0.part
		

		Magnet	Rtarg
		Lookat	Rtarg

	End

	Event

		EName 	Prime1
		Type	start 
		At	Eyel
		
		Bhvr	behaviors/slowprjct1TOROOT2.bhvr
		BhvrOverride
			InitialVelocity 0 0 0
		End
		#Part5	POWERS/DarknessControl/PetrifingPuddleTrail2.Part
		part1	POWERS/DarknessControl/aFearMist.part
		part2	POWERS/DarknessControl/aFearMist2.part
		
		#Part3	POWERS/DarknessControl/aFearTenticles.part
		#Part4	POWERS/DarknessControl/aFearTenticles2.part

		Part	:soulSmoke0.part

		Part	:soulGlow1.part
		Part	:soulGlow0.part
		

		Magnet	Ltarg
		Lookat	Ltarg
	End

End

Condition
	On 	time
	Time	40
	
	Event
		EName 	InitialHeadMist
		Type	Destroy

	End

End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy

	End
	
End

Condition
	On 	Prime1Hit
	
	Event
		EName 	Prime1
		Type	Destroy

	End

End

End			