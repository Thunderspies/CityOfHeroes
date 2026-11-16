#
FxInfo

	Flags InheritAlpha DontSuppress


###########################################################

	#Condition
		#On Time
		#Time 0

		#Event
			#Anim FX_Wings_Phoenix
			#At Back
			#Type Local
			#EName WingsAnchor
			#BhvrOverride
				#TintGeom 1
				#SecondaryTint 100
				#AnimScale 1
			#End
		#End

	#End

	Condition
		On Time
		Time 0

		Event
			At Back
			Type Local
			EName WingsAnchor
			BhvrOverride
				PositionOffset 0 -.2 -0.1
				Scale .9 .9 .9
				TintGeom 1
				#StartColor 255 120 0
			End
			Geom FX_PhoenixWings
		End

		Event
			AltPiv 2
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 0
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 3
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 100
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 4
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 120
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 5
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 120
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 11
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 120
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 12
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 120
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 6
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 0
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 7
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -100
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 8
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -120
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 9
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -120
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 13
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -120
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 14
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -120
			End
			Part GENERIC\PhoenixFire_Out01.part
		End

		Event
			AltPiv 2
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 0
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 3
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 100
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 4
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 120
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 5
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 120
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 11
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 120
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 12
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 120
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 6
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 0
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 7
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -100
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 8
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -120
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 9
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -120
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 13
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -120
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		Event
			AltPiv 14
			At WingsAnchor
			Type Local
			EName FireOne
			BhvrOverride
				PyrRotate 0 0 -120
			End
			Part GENERIC\PhoenixFire_Out02.part
		End

		#Event
			#At Hips
			#Type Local
			#EName TailAnchor
			#BhvrOverride
				#PositionOffset 0 0 -0.4
				#StartColor 255 120 0
			#End
			#Geom FX_PhoenixTail
		#End

		#Event
			#AltPiv 1
			#At TailAnchor
			#Type Local
			#EName FireOne
			#BhvrOverride
				#PyrRotate 60 0 0
			#End
			#Part GENERIC\PhoenixFire_Out01.part
			#Part GENERIC\PhoenixFire_Out02.part
		#End

	End

End
