#############################################################
## FX System Name
#############################################################

FxInfo

#############################################################

Condition

	Event
		Ename	TargetNode
		Type	local
		At	Hips
		BhvrOverride
			PositionOffset	64 -32 12
		End
		#Geom	GEO_FX_TestSphere
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part1	:SmallBeam_Bolt01.part
		Part1	:SmallBeam_Bolt02.part
		Part1	:SmallBeam_Bolt_Glow2.part
		POther	TargetNode
	End		
End

Condition
	On 	Cycle
	Time 	6

	Event
		Type	Local
		At	Origin
		Part1	:SmallBeam_Bolt04.part
		Part1	:SmallBeam_Bolt06.part
		Part1	:SmallBeam_Bolt_Glow2.part
		POther	TargetNode
		Lifespan	60
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part1	:SmallBeam_Bolt04.part
		Part1	:SmallBeam_Bolt06.part
		Part1	:SmallBeam_Bolt_Glow2.part
		POther	TargetNode
		Lifespan	60
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part1	:SmallBeam_Bolt01.part
		Part1	:SmallBeam_Bolt02.part
		Part1	:SmallBeam_Bolt_Glow2.part
		POther	TargetNode
		Lifespan	60
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part1	:SmallBeam_Bolt03.part
		Part1	:SmallBeam_Bolt05.part
		Part1	:SmallBeam_Bolt_Glow1.part
		POther	TargetNode
		Lifespan	60
	End		
End

#############################################################

End