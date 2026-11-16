#########################################################
##	jack
##

FxInfo

Lifespan	200

Flags    InheritAlpha


###############  Center (Tracking) Projectile   #############

Condition

	On 	Time
	Time 	20

	Event
		Type Local
		At origin
		Sound SwordRain2 100.0 100.0 .98
	End
End


Condition
	On 	Time
	Time 	25

	Event
		EName 	Glowie
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset	0 0 1.5
		End
		#Part	:SwordStreak2.part
		Part	:SwordStreak1.part
		Lifespan	7
	End

	Event
		EName 	Glowie2
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	.75 3 2
			pyrRotate	0 -180 0
		End
		Part	:SwordStreak3.part
		Lifespan	3
	End
End

Condition
	On 	Time
	Time 	27

	Event
		EName 	Angle
		Type	Start 
		At	WepR
		LookAt	Target
	End

	Event
		EName 	Sword
		Type	Local 
		At	Angle
		BhvrOverride
			pyrRotate	0 0 0
		End
	End

	Event
		EName 	Prime
		Type	Start
		At	Sword
		Bhvr	:SwordTrackingProjectile.bhvr
		#Geom	GEO_WepR_Sword_1
		Magnet	Target
		Lifespan	60
	End

	Event
		EName 	Offset
		Type	Local 
		At	Prime
		
		Geom	GEO_WepR_Sword_1
		BhvrOverride
			PositionOffset	0 0 -1
		End

		Bhvr	:BladeOffset1.bhvr
	End
	
	Event
		EName 	Glow
		Type	Local 
		At	Offset
		POther	Offset
		
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
		Part	:SwordStreak2.part
	End
End

###########  Secondary Projectiles  #############

Condition
	On 	Time
	Time 	27 #24

	Event
		EName 	Angle1
		Type	Start 
		At	WepR
		LookAt	Target
	End

	Event
		EName 	Sword1
		Type	Local 
		At	Angle1
		BhvrOverride
			pyrRotate	0 -24 0
		End
	End

	Event
		EName 	Prime1
		Type	Local
		At	Sword1
		Bhvr	:SwordConeProjectile.bhvr
		#Geom	GEO_WepR_Sword_1
		Lifespan	30
	End

	Event
		EName 	Offset1
		Type	Local 
		At	Prime1
		
		Geom	GEO_WepR_Sword_1
		BhvrOverride
			PositionOffset	0 0 -1
		End

		Bhvr	:BladeOffset1.bhvr
	End
	
	Event
		EName 	Glow1
		Type	Local 
		At	Offset1
		POther	Offset1
		
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
		Part	:SwordStreak2.part
	End
End

Condition
	On 	Time
	Time 	27 #25

	Event
		EName 	Angle2
		Type	Start 
		At	WepR
		LookAt	Target
	End

	Event
		EName 	Sword2
		Type	Local 
		At	Angle2
		BhvrOverride
			pyrRotate	0 -16 0
		End
	End

	Event
		EName 	Prime2
		Type	Local
		At	Sword2
		Bhvr	:SwordConeProjectile.bhvr
		#Geom	GEO_WepR_Sword_1
		Lifespan	30
	End

	Event
		EName 	Offset2
		Type	Local 
		At	Prime2
		
		Geom	GEO_WepR_Sword_1
		BhvrOverride
			PositionOffset	0 0 -1
		End

		Bhvr	:BladeOffset1.bhvr
	End
	
	Event
		EName 	Glow2
		Type	Local 
		At	Offset2
		POther	Offset2
		
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
		Part	:SwordStreak2.part
	End
End

Condition
	On 	Time
	Time 	27 #26

	Event
		EName 	Angle3
		Type	Start 
		At	WepR
		LookAt	Target
	End

	Event
		EName 	Sword3
		Type	Local 
		At	Angle3
		BhvrOverride
			pyrRotate	0 -8 0
		End
	End

	Event
		EName 	Prime3
		Type	Local
		At	Sword3
		Bhvr	:SwordConeProjectile.bhvr
		#Geom	GEO_WepR_Sword_1
		Lifespan	30
	End

	Event
		EName 	Offset3
		Type	Local 
		At	Prime3
		
		Geom	GEO_WepR_Sword_1
		BhvrOverride
			PositionOffset	0 0 -1
		End

		Bhvr	:BladeOffset1.bhvr
	End
	
	Event
		EName 	Glow3
		Type	Local 
		At	Offset3
		POther	Offset3
		
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
		Part	:SwordStreak2.part
	End
End

Condition
	On 	Time
	Time 	27 #28

	Event
		EName 	Angle4
		Type	Start 
		At	WepR
		LookAt	Target
	End

	Event
		EName 	Sword4
		Type	Local 
		At	Angle4
		BhvrOverride
			pyrRotate	0 8 0
		End
	End

	Event
		EName 	Prime4
		Type	Local
		At	Sword4
		Bhvr	:SwordConeProjectile.bhvr
		#Geom	GEO_WepR_Sword_1
		Lifespan	30
	End

	Event
		EName 	Offset4
		Type	Local 
		At	Prime4
		
		Geom	GEO_WepR_Sword_1
		BhvrOverride
			PositionOffset	0 0 -1
		End

		Bhvr	:BladeOffset1.bhvr
	End
	
	Event
		EName 	Glow4
		Type	Local 
		At	Offset4
		POther	Offset4
		
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
		Part	:SwordStreak2.part
	End
End

Condition
	On 	Time
	Time 	27 #29

	Event
		EName 	Angle5
		Type	Start 
		At	WepR
		LookAt	Target
	End

	Event
		EName 	Sword5
		Type	Local 
		At	Angle5
		BhvrOverride
			pyrRotate	0 16 0
		End
	End

	Event
		EName 	Prime5
		Type	Local
		At	Sword5
		Bhvr	:SwordConeProjectile.bhvr
		#Geom	GEO_WepR_Sword_1
		Lifespan	30
	End

	Event
		EName 	Offset5
		Type	Local 
		At	Prime5
		
		Geom	GEO_WepR_Sword_1
		BhvrOverride
			PositionOffset	0 0 -1
		End

		Bhvr	:BladeOffset1.bhvr
	End
	
	Event
		EName 	Glow5
		Type	Local 
		At	Offset5
		POther	Offset5
		
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
		Part	:SwordStreak2.part
	End
End

Condition
	On 	Time
	Time 	27 #30

	Event
		EName 	Angle6
		Type	Start 
		At	WepR
		LookAt	Target
	End

	Event
		EName 	Sword6
		Type	Local 
		At	Angle6
		BhvrOverride
			pyrRotate	0 24 0
		End
	End

	Event
		EName 	Prime6
		Type	Local
		At	Sword6
		Bhvr	:SwordConeProjectile.bhvr
		#Geom	GEO_WepR_Sword_1
		Lifespan	30
	End

	Event
		EName 	Offset6
		Type	Local 
		At	Prime6
		
		Geom	GEO_WepR_Sword_1
		BhvrOverride
			PositionOffset	0 0 -1
		End

		Bhvr	:BladeOffset1.bhvr
	End
	
	Event
		EName 	Glow6
		Type	Local 
		At	Offset6
		POther	Offset6
		
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
		Part	:SwordStreak2.part
	End
End

######################################################


Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
	End

End

End		