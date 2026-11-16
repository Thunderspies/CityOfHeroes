
FxInfo

Condition
	On 	Time
	Time 	0
	Random	1
	
	Event
		EName 	rocky
		Type	start
		At	Origin
#		LifeSpan 	11
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			StartColor	225 225 255
			Scale			0.125 0.125 0.125
			InitialVelocityJitter	0.2 0.25 0.2
			Gravity			.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9

			PhysDebris		1
			PhysDensity		3
			
			FadeOutLength 		160

		End
				
		#geom	nature_rock_sml_01
		#geom	nature_rock_sml_01
		#geom	nature_rock_sml_02
		#geom	nature_rock_sml_03
		#geom	nature_rock_ledge_flat_sml
		#geom	nature_rock_med

		#geom	FX_Stone01
		#geom	FX_Stone02
		#geom	FX_Stone03
		#geom	FX_Stone04
		#geom	FX_Stone05

		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
	End
	
	Event
		EName 	rocky
		Type	start
		At	Origin
#		LifeSpan 	11
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			StartColor	200 205 255
			Scale			0.125 0.125 0.125
			InitialVelocityJitter	0.2 0.25 0.2
			Gravity			.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9

			PhysDebris		1
			PhysDensity		3
			
			FadeOutLength 		160

		End
				
		#geom	nature_rock_sml_01
		#geom	nature_rock_sml_01
		#geom	nature_rock_sml_02
		#geom	nature_rock_sml_03
		#geom	nature_rock_ledge_flat_sml
		#geom	nature_rock_med

		#geom	FX_Stone01
		#geom	FX_Stone02
		#geom	FX_Stone03
		#geom	FX_Stone04
		#geom	FX_Stone05

		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
	End

End
End