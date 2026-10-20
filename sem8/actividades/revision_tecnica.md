# Autorrevisión técnica asistida

Esta revisión fue realizada por el asistente sobre el código de esta entrega. No es una revisión de un compañero y no asigna una nota académica.

1. Separar propiedad y referencia: ReservaHotel posee la máquina, pero no controla la existencia del huésped ni de la habitación.
2. Mantener semántica de fechas: los intervalos son semiabiertos y se acepta la adyacencia sin solapamiento.
3. Evitar efectos antes de validar: una suspensión inválida no puede cambiar el motivo ni el historial.
4. Distinguir const de noexcept y evitar deducir multiplicidades mínimas del tipo de contenedor.
5. Añadir anotaciones más completas en Python mejoraría los diagramas automáticos; Any señala los tipos no declarados.
6. La notificación se simula y no debe presentarse como correo real ni como una firma electrónica.
